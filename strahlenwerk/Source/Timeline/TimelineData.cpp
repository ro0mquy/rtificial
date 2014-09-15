#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include <StrahlenwerkApplication.h>

TimelineData::TimelineData(const File& dataFile) :
	currentTime(40),
	interpolator(*this)
{
	readTimelineDataFromFile(dataFile);
}

TimelineData::TimelineData() :
	currentTime(40),
	valueTree(treeId::timelineTree),
	interpolator(*this)
{
	for (int i = 0; i < 4; i++) {
		addScene(300 * i, 50 * (i + 1), String(i) + String(41 * i) + ".glsl");
	}

	for (int i = 0; i < 30; i++) {
		ValueTree uniform = addUniform("uniform" + String(i) + String(97 * i), i%2 == 0 ? "color" : "vec3");

		for (int j = 0; j <= (37*i % 4); j++) {
			ValueTree sequence = addSequence(uniform, (j + (97*i % 10)) * 100, 50, "linear");
			addKeyframe(sequence, var((j+1) * 10));
		}
	}
}



TimelineData& TimelineData::getTimelineData() {
	return StrahlenwerkApplication::getInstance()->getProject().getTimelineData();
}

Interpolator& TimelineData::getInterpolator() {
	return interpolator;
}

void TimelineData::readTimelineDataFromFile(const File& dataFile) {
	XmlDocument dataXml(dataFile);
	dataXml.setEmptyTextElementsIgnored(false);
	XmlElement* dataElement = dataXml.getDocumentElement();
	if (dataElement == nullptr) {
		std::cerr << "Failed to parse timeline from file" << std::endl;
		std::cerr << dataXml.getLastParseError() << std::endl;
		valueTree = ValueTree(treeId::timelineTree);
	} else {
		valueTree = ValueTree::fromXml(*dataElement);
		delete dataElement;
	}
}

void TimelineData::writeTimelineDataToFile(const File& dataFile) {
	XmlElement* xmlElement = valueTree.createXml();
	if (!xmlElement->writeToFile(dataFile, "")) {
		std::cerr << "Couldn't write timeline to file" << std::endl;
	}
	delete xmlElement;
}

void TimelineData::addListenerToTree(ValueTree::Listener* listener) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	valueTree.addListener(listener);
}

// comparator function for keyframes in the keyframes array of a sequence
int TimelineData::compareElements(const ValueTree& first, const ValueTree& second) {
		int firstPosition = getKeyframePosition(first);
		int secondPosition = getKeyframePosition(second);
		return firstPosition - secondPosition;
}



// retrieves the scenes array
ValueTree TimelineData::getScenesArray() {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return valueTree.getOrCreateChildWithName(treeId::scenesArray, &undoManager);
}

// returns the number of total scenes
int TimelineData::getNumScenes() {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getScenesArray().getNumChildren();
}


// gets the scene with index nthScene
// returns invalid ValueTree if out of bounds
ValueTree TimelineData::getScene(const int nthScene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getScenesArray().getChild(nthScene);
}

// gets the scene with corresponding sceneId
// returns invalid ValueTree if no scene matches
ValueTree TimelineData::getScene(const var& sceneId) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getScenesArray().getChildWithProperty(treeId::sceneId, sceneId);
}

// checks whether the given valueTree is a scene
bool TimelineData::isScene(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	bool isScene = scene.hasType(treeId::scene);
	isScene &= scene.hasProperty(treeId::sceneId);
	isScene &= scene.hasProperty(treeId::sceneStart);
	isScene &= scene.hasProperty(treeId::sceneDuration);
	isScene &= scene.hasProperty(treeId::sceneShaderSource);
	return isScene;
}

// adds a scene to the scenes array at a given position
// returns the scene
// position defaults to -1 (append to end)
ValueTree TimelineData::addScene(ValueTree scene, int position) {
	jassert(isScene(scene));
	addSceneUnchecked(scene, position);
	return scene;
}

// adds a scene with the given vars at position
// returns the assembled scene ValueTree
// position defaults to -1 (append to end)
ValueTree TimelineData::addScene(var start, var duration, var shaderSource, int position) {
	var id = getNewSceneId();
	ValueTree scene(treeId::scene);
	setSceneId(scene, id);
	setSceneStart(scene, start);
	setSceneDuration(scene, duration);
	setSceneShaderSource(scene, shaderSource);
	addSceneUnchecked(scene, position);
	return scene;
}

// adds a scene to the scenes array
// returns the scene again
// doesn't perform any checking (you should use addScene(ValueTree))
// position defaults to -1 (append to end)
ValueTree TimelineData::addSceneUnchecked(ValueTree scene, int position) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getScenesArray().addChild(scene, position, &undoManager);
	return scene;
}

// removes a scene from the scenes array
void TimelineData::removeScene(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getScenesArray().removeChild(scene, &undoManager);
}


// gets the id of a scene
var TimelineData::getSceneId(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return scene.getProperty(treeId::sceneId);
}

// gets the start time of a scene
var TimelineData::getSceneStart(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return scene.getProperty(treeId::sceneStart);
}

// gets the duration of a scene
var TimelineData::getSceneDuration(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return scene.getProperty(treeId::sceneDuration);
}

// gets the shaderSource of a scene
var TimelineData::getSceneShaderSource(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return scene.getProperty(treeId::sceneShaderSource);
}


// gets the shaderSource of a scene as a value
Value TimelineData::getSceneShaderSourceAsValue(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return scene.getPropertyAsValue(treeId::sceneShaderSource, &undoManager);
}


// sets the id for the given scene
void TimelineData::setSceneId(ValueTree scene, var id) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	scene.setProperty(treeId::sceneId, id, &undoManager);
}

// sets the start time for the given scene
void TimelineData::setSceneStart(ValueTree scene, var start) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	scene.setProperty(treeId::sceneStart, start, &undoManager);
}

// sets the duration for the given scene
void TimelineData::setSceneDuration(ValueTree scene, var duration) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	scene.setProperty(treeId::sceneDuration, duration, &undoManager);
}

// sets the shaderSource for the given scene
void TimelineData::setSceneShaderSource(ValueTree scene, var shaderSource) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	scene.setProperty(treeId::sceneShaderSource, shaderSource, &undoManager);
}


// finds the the end time of the last scene
int TimelineData::getLastSceneEndTime() {
	const int numScenes = getNumScenes();
	int maxEndTime = 0;

	for (int i = 0; i < numScenes; i++) {
		ValueTree scene = getScene(i);
		const int start = getSceneStart(scene);
		const int duration = getSceneDuration(scene);
		const int end = start + duration;
		maxEndTime = jmax(maxEndTime, end);
	}

	return maxEndTime;
}

// returns the active scene for a timepoint
// a invalid tree is returned if there is none
ValueTree TimelineData::getSceneForTime(const int absoluteTime) {
	const int numScenes = getNumScenes();
	/*
	// needs convertion to new TimelineData functions
	// matches if the time is inside the scene and also if after the end
	int smallestDistance = INT_MAX;
	int bestScene = 0;

	for (int i = 0; i < numScenes; i++) {
		ValueTree scene = scenesArray.getChild(i);
		const int start = scene.getProperty(treeId::sceneStart);
		const int distance = absoluteTime - start;
		if (isPositiveAndBelow(distance, smallestDistance)) {
			smallestDistance = distance;
			bestScene = i;
		}
	}

	return scenesArray.getChild(bestScene);
	// */

	//*
	// matches only if inside scene
	for (int i = 0; i < numScenes; i++) {
		ValueTree scene = getScene(i);
		const int start = getSceneStart(scene);
		const int duration = getSceneDuration(scene);
		const int distance = absoluteTime - start;
		if (isPositiveAndBelow(distance, duration)) {
			return scene;
		}
	}
	// in case of no matching scene, return invalid tree
	return ValueTree();
	// */
}

// returns the scene that should be displayed at the current time
ValueTree TimelineData::getCurrentScene() {
	return getSceneForTime(currentTime.getValue());
}

// returns a scene Id that is currently not used
int TimelineData::getNewSceneId() {
	int biggestId = 99; // some room for $stuff
	const int numScenes = getNumScenes();

	for (int i = 0; i < numScenes; i++) {
		ValueTree scene = getScene(i);
		const int currentId = getSceneId(scene);
		biggestId = jmax(biggestId, currentId);
	}
	// we need a unused id, so increment by one;
	return biggestId + 1;
}



// retrieves the uniforms array
ValueTree TimelineData::getUniformsArray() {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return valueTree.getOrCreateChildWithName(treeId::uniformsArray, &undoManager);
}

// returns the number of total uniforms
int TimelineData::getNumUniforms() {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getUniformsArray().getNumChildren();
}


// gets the uniform with index nthUniform
// returns invalid ValueTree if out of bounds
ValueTree TimelineData::getUniform(const int nthUniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getUniformsArray().getChild(nthUniform);
}

// returns the uniform with the given name
// returns invalid ValueTree if no uniforms with the name exists
ValueTree TimelineData::getUniform(const var& name) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getUniformsArray().getChildWithProperty(treeId::uniformName, name);
}

// checks the ValueTree for all requirements to be a uniform
bool TimelineData::isUniform(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	bool isUniform = uniform.hasType(treeId::uniform);
	isUniform &= uniform.hasProperty(treeId::uniformName);
	isUniform &= uniform.hasProperty(treeId::uniformType);
	isUniform &= getUniformStandardValue(uniform).isValid(); // TODO: real checking
	return isUniform;
}

// adds a uniform to the uniforms array at a given position
// returns the uniform again
// position defaults to -1 (append to end)
ValueTree TimelineData::addUniform(ValueTree uniform, int position) {
	jassert(isUniform(uniform));
	addUniformUnchecked(uniform, position);
	return uniform;
}

// adds a uniform with the given vars at position
// returns the assembled uniform
// position defaults to -1 (append to end)
ValueTree TimelineData::addUniform(var name, var type, int position) {
	ValueTree uniform(treeId::uniform);
	setUniformName(uniform, name);
	setUniformType(uniform, type);
	initializeValue(getOrCreateUniformStandardValue(uniform), type);
	addUniformUnchecked(uniform, position);
	return uniform;
}

// adds a uniform to the uniforms array
// returns the uniform again
// doesn't perform any checking (you should use addUniform(ValueTree))
// position defaults to -1 (append to end)
ValueTree TimelineData::addUniformUnchecked(ValueTree uniform, int position) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getUniformsArray().addChild(uniform, position, &undoManager);
	return uniform;
}


// gets the name of a uniform
var TimelineData::getUniformName(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return uniform.getProperty(treeId::uniformName);
}

// gets the type of a uniform
var TimelineData::getUniformType(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return uniform.getProperty(treeId::uniformType);
}

// gets the standard value for a uniform
ValueTree TimelineData::getUniformStandardValue(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return uniform.getChildWithName(treeId::uniformStandardValue);
}

// gets or creates the standard value for a uniform
ValueTree TimelineData::getOrCreateUniformStandardValue(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return uniform.getOrCreateChildWithName(treeId::uniformStandardValue, &undoManager);
}


// sets the name for the given uniform
void TimelineData::setUniformName(ValueTree uniform, var name) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	uniform.setProperty(treeId::uniformName, name, &undoManager);
}

// sets the type for the given uniform
void TimelineData::setUniformType(ValueTree uniform, var type) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	uniform.setProperty(treeId::uniformType, type, &undoManager);
}

// sets the standard value for the given uniform
void TimelineData::setUniformStandardValue(ValueTree uniform, ValueTree standardValue) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	uniform.removeChild(uniform.getChildWithName(treeId::uniformStandardValue), &undoManager);
	uniform.addChild(standardValue, 0, &undoManager); // TODO: check for type
}



// retrieves the sequences array for a given uniform
ValueTree TimelineData::getSequencesArray(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return uniform.getOrCreateChildWithName(treeId::sequencesArray, &undoManager);
}

// returns the total number of sequences for a uniform
int TimelineData::getNumSequences(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getSequencesArray(uniform).getNumChildren();
}


// gets the sequence with index nthSequence of a uniform
// returns invalid ValueTree if out of bounds
ValueTree TimelineData::getSequence(ValueTree uniform, const int nthSequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getSequencesArray(uniform).getChild(nthSequence);
}

// checks the ValueTree for all requirements to be a sequence
bool TimelineData::isSequence(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	bool isSequence = sequence.hasType(treeId::sequence);
	isSequence &= sequence.hasProperty(treeId::sequenceSceneId);
	isSequence &= sequence.hasProperty(treeId::sequenceStart);
	isSequence &= sequence.hasProperty(treeId::sequenceDuration);
	isSequence &= sequence.hasProperty(treeId::sequenceInterpolation);
	isSequence &= sequence.getChildWithName(treeId::keyframesArray).isValid(); // TODO: check exact type
	return isSequence;
}

// adds a sequence to the sequences array of a uniform at a given position
// don't initializes the keyframes array
// returns the sequence again
// position defaults to -1 (append to end)
ValueTree TimelineData::addSequence(ValueTree uniform, ValueTree sequence, int position) {
	jassert(isSequence(sequence));
	addSequenceUnchecked(uniform, sequence, position);
	return sequence;
}

// adds a sequence with the given vars to a uniform at position
// returns the assembled sequence
// position defaults to -1 (append to end)
ValueTree TimelineData::addSequence(ValueTree uniform, int absoluteStart, var duration, var interpolation, int position) {
	ValueTree sequence(treeId::sequence);
	setSequencePropertiesForAbsoluteStart(sequence, absoluteStart);
	setSequenceDuration(sequence, duration);
	setSequenceInterpolation(sequence, interpolation);
	addSequenceUnchecked(uniform, sequence, position);
	initializeKeyframesArray(sequence);
	return sequence;
}

// adds a sequence to the sequences array of a uniform
// returns the sequence again
// doesn't perform any checking (you should use addSequence(ValueTree))
// position defaults to -1 (append to end)
ValueTree TimelineData::addSequenceUnchecked(ValueTree uniform, ValueTree sequence, int position) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getSequencesArray(uniform).addChild(sequence, position, &undoManager);
	return sequence;
}

// removes a sequence from it's uniform parent
void TimelineData::removeSequence(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	sequence.getParent().removeChild(sequence, &undoManager);
}


// gets the sceneId of a sequence
var TimelineData::getSequenceSceneId(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getProperty(treeId::sequenceSceneId);
}

// gets the start time of a sequence
var TimelineData::getSequenceStart(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getProperty(treeId::sequenceStart);
}

// gets the duration of a sequence
var TimelineData::getSequenceDuration(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getProperty(treeId::sequenceDuration);
}

// gets the interpolation method of a sequence
var TimelineData::getSequenceInterpolation(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getProperty(treeId::sequenceInterpolation);
}


// sets the sceneId for the given sequence
void TimelineData::setSequenceSceneId(ValueTree sequence, var sceneId) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	sequence.setProperty(treeId::sequenceSceneId, sceneId, &undoManager);
}

// sets the start time for the given sequence
void TimelineData::setSequenceStart(ValueTree sequence, var start) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	sequence.setProperty(treeId::sequenceStart, start, &undoManager);
}

// sets the duration for the given sequence
void TimelineData::setSequenceDuration(ValueTree sequence, var duration) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	sequence.setProperty(treeId::sequenceDuration, duration, &undoManager);

	// also update the time position of the last keyframe
	const int numKeyframes = getNumKeyframes(sequence);
	if (numKeyframes < 2) {
		// keyframes array is not initialized
		return;
	}
	ValueTree lastKeyframe = getKeyframe(sequence, numKeyframes - 1);
	setKeyframePosition(lastKeyframe, duration);
}

// sets the interpolation method for the given sequence
void TimelineData::setSequenceInterpolation(ValueTree sequence, var interpolation) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	sequence.setProperty(treeId::sequenceInterpolation, interpolation, &undoManager);
}


// sets the sceneId and relative start time of a sequence for a given absolute start time
// return true when the scene for this sequence is a new one, false otherwise
void TimelineData::setSequencePropertiesForAbsoluteStart(ValueTree sequence, int absoluteStart) {
			ValueTree sceneForSequence = getSceneForTime(absoluteStart);
			var sceneId = getSceneId(sceneForSequence);
			setSequenceSceneId(sequence, sceneId);

			const int sceneStart = getSceneStart(sceneForSequence);
			var relativeStart = absoluteStart - sceneStart;
			setSequenceStart(sequence, relativeStart);
}

// returns the start time of the sequence in absolute time
// and not relative to the scene start time
int TimelineData::getAbsoluteStartForSequence(ValueTree sequence) {
	const int sceneStart = getSceneStart(getScene(getSequenceSceneId(sequence)));
	const int sequenceStart = getSequenceStart(sequence);
	return sceneStart + sequenceStart;
}

// returns the uniform the sequence belongs to
// of course the sequence must already be added to one
ValueTree TimelineData::getSequenceParentUniform(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getParent().getParent();
}



// returns the keyframes array for a sequence
ValueTree TimelineData::getKeyframesArray(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getOrCreateChildWithName(treeId::keyframesArray, &undoManager);
}

// returns the total number of keyframes for a sequence
int TimelineData::getNumKeyframes(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getKeyframesArray(sequence).getNumChildren();
}


// gets the keyframe with index nthKeyframe of a sequence
// returns invalid ValueTree if out of bounds
ValueTree TimelineData::getKeyframe(ValueTree sequence, const int nthKeyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getKeyframesArray(sequence).getChild(nthKeyframe);
}

// checks the ValueTree for all requirements to be a sequence
bool TimelineData::isKeyframe(ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	bool isKeyframe = keyframe.hasType(treeId::keyframe);
	isKeyframe &= keyframe.hasProperty(treeId::keyframePosition);
	isKeyframe &= getKeyframeValue(keyframe).isValid();

	/*
	// TODO: check the uniform against the keyframe type
	// maybe in own function
	ValueTree keyframeValue = keyframe.getChildWithName(treeId::keyframeValue);
	String uniformType = sequence.getParent().getParent().getProperty(treeId::uniformType);
	if (keyframeValue.hasProperty(treeId::valueBoolState)) {
		isKeyframe &= uniformType.equalsIgnoreCase("bool");
	} else if (keyframeValue.hasProperty(treeId::valueFloatX)) {
		isKeyframe &= uniformType.equalsIgnoreCase("float");
	} else if (keyframeValue.hasProperty(treeId::valueVec2X)) {
		isKeyframe &= uniformType.equalsIgnoreCase("vec2");
	} else if (keyframeValue.hasProperty(treeId::valueVec3X)) {
		isKeyframe &= uniformType.equalsIgnoreCase("vec3");
	} else if (keyframeValue.hasProperty(treeId::valueColorR)) {
		isKeyframe &= uniformType.equalsIgnoreCase("color");
	} else {
		isKeyframe = false;
	}
	*/
	return isKeyframe;
}

// adds a keyframe to the keyframes array of a sequence at a sorted position
// returns the keyframe again
// sequence must already be added to a uniform
ValueTree TimelineData::addKeyframe(ValueTree sequence, ValueTree keyframe) {
	jassert(isKeyframe(keyframe));
	addKeyframeUnchecked(sequence, keyframe);
	return keyframe;
}

// adds a keyframe with the given time position to a sequence at a sorted position
// sequence must already be added to a uniform
// initializes the keyframe with a zero value
// returns the new keyframe
ValueTree TimelineData::addKeyframe(ValueTree sequence, var keyframePosition) {
	ValueTree keyframe(treeId::keyframe);
	setKeyframePosition(keyframe, keyframePosition);

	var uniformType = getUniformType(getSequenceParentUniform(sequence));
	initializeValue(getKeyframeValue(keyframe), uniformType);

	addKeyframeUnchecked(sequence, keyframe);
	return keyframe;
}

// adds a keyframe to the keyframes array of a sequence
// returns the keyframe again
// doesn't perform any checking (you should use addKeyframe(ValueTree))
// keyframe gets inserted at a sorted position
ValueTree TimelineData::addKeyframeUnchecked(ValueTree sequence, ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	ValueTree keyframesArray = getKeyframesArray(sequence);
	keyframesArray.addChild(keyframe, -1, nullptr);
	keyframesArray.sort<TimelineData>(*this, &undoManager, true);
	return keyframe;
}

// removes a keyframe from it's sequence parent
void TimelineData::removeKeyframe(ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	keyframe.getParent().removeChild(keyframe, &undoManager);
}


// gets the time position of a keyframe
var TimelineData::getKeyframePosition(ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return keyframe.getProperty(treeId::keyframePosition);
}

// gets the value for a keyframe
ValueTree TimelineData::getKeyframeValue(ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return keyframe.getOrCreateChildWithName(treeId::keyframeValue, &undoManager);
}


// sets the time position for the given keyframe
void TimelineData::setKeyframePosition(ValueTree keyframe, var position) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	keyframe.setProperty(treeId::keyframePosition, position, &undoManager);
}

// sets the value for the given keyframe
void TimelineData::setKeyframeValue(ValueTree keyframe, ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	keyframe.removeChild(keyframe.getChildWithName(treeId::keyframeValue), &undoManager);
	keyframe.addChild(value, 0, &undoManager); // TODO: check for type
}


// initialize the keyframesArray of a sequence
// creates the start and end keyframe
// sequence must already be added to a uniform
void TimelineData::initializeKeyframesArray(ValueTree sequence) {
	jassert(getSequenceParentUniform(sequence).isValid());

	var relativeEndTime = getSequenceDuration(sequence);
	addKeyframe(sequence, 0);
	addKeyframe(sequence, relativeEndTime);
}



// returns the number of properties in this value
int TimelineData::getNumValueProperties(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getNumProperties();
}

// initialize a value with the specified type inside valueData
// return false when valueData was already initialized or the type is unknown
bool TimelineData::initializeValue(ValueTree valueData, String valueType) {
	if (getNumValueProperties(valueData) != 0) {
		// value is already populated
		return false;
	}

	if (valueType == "bool") {
		setValueBoolState(valueData, false);
	} else if (valueType == "float") {
		setValueFloatX(valueData, 0.);
	} else if (valueType == "vec2") {
		setValueVec2X(valueData, 0.);
		setValueVec2Y(valueData, 0.);
	} else if (valueType == "vec3") {
		setValueVec3X(valueData, 0.);
		setValueVec3Y(valueData, 0.);
		setValueVec3Z(valueData, 0.);
	} else if (valueType == "color") {
		setValueColorR(valueData, 0.);
		setValueColorG(valueData, 0.);
		setValueColorB(valueData, 0.);
	} else {
		return false;
	}
	return true;
}


// checks if the value is of type bool
bool TimelineData::isValueBool(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueBoolState);
}

// checks if the value is of type float
bool TimelineData::isValueFloat(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueFloatX);
}

// checks if the value is of type vec2
bool TimelineData::isValueVec2(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueVec2X);
}

// checks if the value is of type vec3
bool TimelineData::isValueVec3(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueVec3X);
}

// checks if the value is of type color
bool TimelineData::isValueColor(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueColorR);
}


// get boolState of a bool value
var TimelineData::getValueBoolState(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueBoolState);
}

// get floatX of a float value
var TimelineData::getValueFloatX(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueFloatX);
}

// get vec2X of a vec2 value
var TimelineData::getValueVec2X(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec2X);
}

// get vec2Y of a vec2 value
var TimelineData::getValueVec2Y(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec2Y);
}

// get vec3X of a vec3 value
var TimelineData::getValueVec3X(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec3X);
}

// get vec3Y of a vec3 value
var TimelineData::getValueVec3Y(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec3Y);
}

// get vec3Z of a vec3 value
var TimelineData::getValueVec3Z(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec3Z);
}

// get colorR of a color value
var TimelineData::getValueColorR(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueColorR);
}

// get colorG of a color value
var TimelineData::getValueColorG(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueColorG);
}

// get colorB of a color value
var TimelineData::getValueColorB(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueColorB);
}


// get boolState of a bool value as a juce::Value
Value TimelineData::getValueBoolStateAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueBoolState, &undoManager);
}

// get floatX of a float value as a juce::Value
Value TimelineData::getValueFloatXAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueFloatX, &undoManager);
}

// get vec2X of a vec2 value as a juce::Value
Value TimelineData::getValueVec2XAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec2X, &undoManager);
}

// get vec2Y of a vec2 value as a juce::Value
Value TimelineData::getValueVec2YAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec2Y, &undoManager);
}

// get vec3X of a vec3 value as a juce::Value
Value TimelineData::getValueVec3XAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec3X, &undoManager);
}

// get vec3Y of a vec3 value as a juce::Value
Value TimelineData::getValueVec3YAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec3Y, &undoManager);
}

// get vec3Z of a vec3 value as a juce::Value
Value TimelineData::getValueVec3ZAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec3Z, &undoManager);
}

// get colorR of a color value as a juce::Value
Value TimelineData::getValueColorRAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueColorR, &undoManager);
}

// get colorG of a color value as a juce::Value
Value TimelineData::getValueColorGAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueColorG, &undoManager);
}

// get colorB of a color value as a juce::Value
Value TimelineData::getValueColorBAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueColorB, &undoManager);
}


// TODO: some checking maybe
// set boolState of a bool value
void TimelineData::setValueBoolState(ValueTree value, var boolState) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueBoolState, boolState, &undoManager);
}

// set floatX of a float value
void TimelineData::setValueFloatX(ValueTree value, var floatX) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueFloatX, floatX, &undoManager);
}

// set vec2X of a vec2 value
void TimelineData::setValueVec2X(ValueTree value, var vec2X) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec2X, vec2X, &undoManager);
}

// set vec2Y of a vec2 value
void TimelineData::setValueVec2Y(ValueTree value, var vec2Y) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec2Y, vec2Y, &undoManager);
}

// set vec3X of a vec3 value
void TimelineData::setValueVec3X(ValueTree value, var vec3X) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec3X, vec3X, &undoManager);
}

// set vec3Y of a vec3 value
void TimelineData::setValueVec3Y(ValueTree value, var vec3Y) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec3Y, vec3Y, &undoManager);
}

// set vec3Z of a vec3 value
void TimelineData::setValueVec3Z(ValueTree value, var vec3Z) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec3Z, vec3Z, &undoManager);
}

// set colorR of a color value
void TimelineData::setValueColorR(ValueTree value, var colorR) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueColorR, colorR, &undoManager);
}

// set colorG of a color value
void TimelineData::setValueColorG(ValueTree value, var colorG) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueColorG, colorG, &undoManager);
}

// set colorB of a color value
void TimelineData::setValueColorB(ValueTree value, var colorB) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueColorB, colorB, &undoManager);
}

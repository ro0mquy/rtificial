#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <AudioManager.h>
#include <glm/glm.hpp>
#include "Splines.h"
#include "ZoomFactor.h"
#include "JsonExporter.h"

TimelineData::TimelineData(const File& dataFile) :
	interpolator(*this)
{
	MainWindow::getApplicationCommandManager().addListener(this);
	readTimelineDataFromFile(dataFile);
}

TimelineData::TimelineData() :
	valueTree(treeId::timelineTree),
	interpolator(*this)
{
	MainWindow::getApplicationCommandManager().addListener(this);
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

// use this if you want to operate directly on the data
// this may be helpful:
//  std::lock_guard<std::recursive_mutex> lock(data.getMutex());
//  it locks for the current context and unlocks on destruction
std::recursive_mutex& TimelineData::getMutex() {
	return treeMutex;
}

UndoManager& TimelineData::getUndoManager() {
	return undoManager;
}

Selection& TimelineData::getSelection() {
	return selection;
}

void TimelineData::readTimelineDataFromFile(const File& dataFile) {
	var jsonRepresentation;
	const Result result = JSON::parse(dataFile.loadFileAsString(), jsonRepresentation);

	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getUndoManager().clearUndoHistory();
	getSelection().clear();

	if (result.wasOk()) {
		valueTree = JsonExporter::fromJson(jsonRepresentation, treeId::timelineTree);
	} else {
		std::cerr << "Failed to parse timeline from file\n";
		std::cerr << result.getErrorMessage() << '\n';
		valueTree = ValueTree(treeId::timelineTree);
	}
}

void TimelineData::writeTimelineDataToFile(const File& dataFile) {
	treeMutex.lock();
	const var jsonRepresentation = JsonExporter::toJson(valueTree);
	treeMutex.unlock();
	// write to a temporary file and replace the original when successful
	// to avoid corrupting files
	TemporaryFile tempFile(dataFile);
	const std::unique_ptr<FileOutputStream> stream(tempFile.getFile().createOutputStream());
	if (stream != nullptr) {
		JSON::writeToStream(*stream, jsonRepresentation);
		if (tempFile.overwriteTargetFileWithTemporary()) {
			return;
		}
	}
	std::cerr << "Couldn't write timeline to file" << std::endl;
}

void TimelineData::addListenerToTree(ValueTree::Listener* listener) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	valueTree.addListener(listener);
}

void TimelineData::removeListenerFromTree(ValueTree::Listener* listener) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	valueTree.removeListener(listener);
}

void TimelineData::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case TimelineData::undoAction:
			getUndoManager().undo();
			break;
		case TimelineData::redoAction:
			getUndoManager().redo();
			break;
	}
}

void TimelineData::applicationCommandListChanged() {
}



// retrieves the scenes array
ValueTree TimelineData::getScenesArray() {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return valueTree.getOrCreateChildWithName(treeId::scenesArray, nullptr);
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
// position defaults to -1 (add sorted)
ValueTree TimelineData::addScene(ValueTree scene, int position) {
	jassert(isScene(scene));
	addSceneUnchecked(scene, position);
	return scene;
}

// adds a scene with the given vars at position
// returns the assembled scene ValueTree
// position defaults to -1 (add sorted)
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
// position defaults to -1 (add sorted)
ValueTree TimelineData::addSceneUnchecked(ValueTree scene, int position) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	if (position >= 0) {
		getScenesArray().addChild(scene, position, &undoManager);
		return scene;
	}

	const int numScenes = getNumScenes();
	int sortedPosition = 0;
	for (; sortedPosition < numScenes; sortedPosition++) {
		ValueTree otherScene = getScene(sortedPosition);
		if (compareScenes(scene, otherScene) < 0) {
			// break if the new scene comes before the currently checked one
			break;
		}
		// if the new scene comes last sortedPosition will contain numScenes
		// and the new scene will be added at the end
	}
	getScenesArray().addChild(scene, sortedPosition, &undoManager);
	return scene;
}

// removes a scene from the scenes array
void TimelineData::removeScene(ValueTree scene) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getScenesArray().removeChild(scene, &undoManager);

	// remove the sceneId of the scene to be deleted
	// from the sequences belonging to this scene
	const var& sceneId = getSceneId(scene);
	const int sceneStart = getSceneStart(scene);
	const int numUniforms = getNumUniforms();

	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = getUniform(i);
		const int numSequences = getNumSequences(uniform);

		for (int j = 0; j < numSequences; j++) {
			ValueTree sequence = getSequence(uniform, j);

			if (getSequenceSceneId(sequence) != sceneId) {
				continue;
			}

			const int sequenceStart = getSequenceStart(sequence);
			const int absoluteStart = sceneStart + sequenceStart;
			setSequencePropertiesForAbsoluteStart(sequence, absoluteStart);
		}
	}
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

	// check if some existing sequences without scene now belong to this scene
	const int sceneStart = start;
	const int sceneDuration = getSceneDuration(scene);
	const int numUniforms = getNumUniforms();

	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = getUniform(i);
		const int numSequences = getNumSequences(uniform);

		for (int j = 0; j < numSequences; j++) {
			ValueTree sequence = getSequence(uniform, j);

			if (getSequenceSceneId(sequence) != var::null) {
				continue;
			}

			const int absoluteStart = getAbsoluteStartForSequence(sequence);
			const int relativeStart = absoluteStart - sceneStart;

			if (isPositiveAndBelow(relativeStart, sceneDuration)) {
				setSequencePropertiesForAbsoluteStart(sequence, absoluteStart);
			}
		}
	}
}

// sets the duration for the given scene
void TimelineData::setSceneDuration(ValueTree scene, var duration) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	scene.setProperty(treeId::sceneDuration, duration, &undoManager);

	// check if some existing sequences without scene now belong to this scene
	const int sceneStart = getSceneStart(scene);
	const int sceneDuration = duration;
	const int numUniforms = getNumUniforms();

	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = getUniform(i);
		const int numSequences = getNumSequences(uniform);

		for (int j = 0; j < numSequences; j++) {
			ValueTree sequence = getSequence(uniform, j);

			if (getSequenceSceneId(sequence) != var::null) {
				continue;
			}

			const int absoluteStart = getAbsoluteStartForSequence(sequence);
			const int relativeStart = absoluteStart - sceneStart;

			if (isPositiveAndBelow(relativeStart, sceneDuration)) {
				setSequencePropertiesForAbsoluteStart(sequence, absoluteStart);
			}
		}
	}
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
	return getSceneForTime(AudioManager::getAudioManager().getTime());
}

// returns a scene Id that is currently not used
int TimelineData::getNewSceneId() {
	Random rand;
	int candidateId = 100;
	bool idInUse = true;
	int counter = 0;
	const int numScenes = getNumScenes();

	while (idInUse) {
		candidateId = rand.nextInt(Range<int>(100, 1000));

		idInUse = false;
		for (int i = 0; i < numScenes; i++) {
			ValueTree scene = getScene(i);
			const int currentId = getSceneId(scene);
			if (candidateId == currentId) {
				idInUse = true;
				break;
			}
		}

		counter++;
		if (counter > 100000) {
			// all IDs from 100 to 1000 in use?
			jassertfalse;
			break;
		}
	}

	return candidateId;
}

// comparator function for scenes
// < 0 if first before second, = 0 if equal, > 0 if first after second
int TimelineData::compareScenes(const ValueTree& first, const ValueTree& second) {
	const int firstId = getSceneId(first);
	const int secondId = getSceneId(second);
	return firstId - secondId;
}



// retrieves the uniforms array
ValueTree TimelineData::getUniformsArray() {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return valueTree.getOrCreateChildWithName(treeId::uniformsArray, nullptr);
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

// adds a uniform to the uniforms array at a sorted position
// returns the uniform again
ValueTree TimelineData::addUniform(ValueTree uniform) {
	jassert(isUniform(uniform));
	addUniformUnchecked(uniform);
	return uniform;
}

// adds a uniform with the given vars at a sorted position
// returns the assembled uniform
ValueTree TimelineData::addUniform(var name, var type) {
	ValueTree uniform(treeId::uniform);
	setUniformName(uniform, name);
	setUniformType(uniform, type);
	initializeValue(getOrCreateUniformStandardValue(uniform), type);
	addUniformUnchecked(uniform);
	return uniform;
}

// adds a uniform to the uniforms array at a sorted position
// returns the uniform again
// doesn't perform any checking (you should use addUniform(ValueTree))
ValueTree TimelineData::addUniformUnchecked(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	const int numUniforms = getNumUniforms();
	int sortedPosition = 0;
	for (; sortedPosition < numUniforms; sortedPosition++) {
		ValueTree otherUniform = getUniform(sortedPosition);
		if (compareUniforms(uniform, otherUniform) < 0) {
			// break if the new uniform comes before the currently checked one
			break;
		}
		// if the new uniform comes last sortedPosition will contain numUniforms
		// and the new uniform will be added at the end
	}
	getUniformsArray().addChild(uniform, sortedPosition, &undoManager);
	return uniform;
}

// removes a uniform from the uniformsArray
void TimelineData::removeUniform(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	getUniformsArray().removeChild(uniform, &undoManager);
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
	return uniform.getOrCreateChildWithName(treeId::uniformStandardValue, nullptr);
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


// returns the index of the uniform in the uniformsArray
int TimelineData::getUniformIndex(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getUniformsArray().indexOf(uniform);
}

// comparator function for uniforms in the uniforms array
int TimelineData::compareUniforms(const ValueTree& first, const ValueTree& second) {
		String firstName = getUniformName(first);
		String secondName = getUniformName(second);
		return firstName.compareNatural(secondName);
}

void TimelineData::bakeUniform(ValueTree uniform) {
	const String name = getUniformName(uniform);
	const String value = getValueAsString(getUniformStandardValue(uniform));

	const File bakefile = StrahlenwerkApplication::getInstance()->getProject().getLoader().getBakeFile();
	bakefile.appendText(name + " " + value + "\n");

	removeUniform(uniform);
}



// retrieves the sequences array for a given uniform
ValueTree TimelineData::getSequencesArray(ValueTree uniform) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return uniform.getOrCreateChildWithName(treeId::sequencesArray, nullptr);
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
// position defaults to -1 (add sorted)
ValueTree TimelineData::addSequence(ValueTree uniform, ValueTree sequence, int position) {
	jassert(isSequence(sequence));
	addSequenceUnchecked(uniform, sequence, position);
	return sequence;
}

// adds a sequence with the given vars to a uniform at position
// returns the assembled sequence
// position defaults to -1 (add sorted)
ValueTree TimelineData::addSequence(ValueTree uniform, int absoluteStart, var duration, var interpolation, int position) {
	ValueTree sequence(treeId::sequence);
	setSequencePropertiesForAbsoluteStart(sequence, absoluteStart);
	setSequenceDuration(sequence, duration);
	setSequenceInterpolation(sequence, interpolation);
	addSequenceUnchecked(uniform, sequence, position);
	return sequence;
}

// adds a sequence to the sequences array of a uniform
// returns the sequence again
// doesn't perform any checking (you should use addSequence(ValueTree))
// position defaults to -1 (add sorted)
ValueTree TimelineData::addSequenceUnchecked(ValueTree uniform, ValueTree sequence, int position) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	if (position >= 0) {
		getSequencesArray(uniform).addChild(sequence, position, &undoManager);
		return sequence;
	}

	const int numSequences = getNumSequences(uniform);
	int sortedPosition = 0;
	for (; sortedPosition < numSequences; sortedPosition++) {
		ValueTree otherSequence = getSequence(uniform, sortedPosition);
		if (compareSequences(sequence, otherSequence) < 0) {
			// break if the new sequence comes before the currently checked one
			break;
		}
		// if the new sequence comes last sortedPosition will contain numSequences
		// and the new sequence will be added at the end
	}
	getSequencesArray(uniform).addChild(sequence, sortedPosition, &undoManager);
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

// returns the active sequence of a uniform for a timepoint
// a invalid tree is returned if there is none
ValueTree TimelineData::getSequenceForTime(ValueTree uniform, const int absoluteTime) {
	const int numSequences = getNumSequences(uniform);
	for (int i = 0; i < numSequences; i++) {
		ValueTree sequence = getSequence(uniform, i);
		const int start = getAbsoluteStartForSequence(sequence);
		const int duration = getSequenceDuration(sequence);
		const int relativeTime = absoluteTime - start;
		if (isPositiveAndNotGreaterThan(relativeTime, duration)) {
			return sequence;
		}
	}
	// in case of no matching sequence, return invalid tree
	return ValueTree();
}

// returns the sequence of a uniform that should be used at the current time
ValueTree TimelineData::getCurrentSequence(ValueTree uniform) {
	return getSequenceForTime(uniform, AudioManager::getAudioManager().getTime());
}

// returns the uniform the sequence belongs to
// of course the sequence must already be added to one
ValueTree TimelineData::getSequenceParentUniform(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getParent().getParent();
}

// returns the standard value of the uniform this sequence belongs to
ValueTree TimelineData::getSequenceUniformStandardValue(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return getUniformStandardValue(getSequenceParentUniform(sequence));
}


// comparator function for sequences
// sort at first for sceneIds, then for start time
// < 0 if first before second, = 0 if equal, > 0 if first after second
int TimelineData::compareSequences(const ValueTree& first, const ValueTree& second) {
	const int firstId = getSequenceSceneId(first);
	const int secondId = getSequenceSceneId(second);
	const int difference = firstId - secondId;

	if (difference != 0) {
		return difference;
	}

	// same sceneId, compare start times
	const int firstStart = getSequenceStart(first);
	const int secondStart = getSequenceStart(second);
	return firstStart - secondStart;
}



// returns the keyframes array for a sequence
ValueTree TimelineData::getKeyframesArray(ValueTree sequence) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return sequence.getOrCreateChildWithName(treeId::keyframesArray, nullptr);
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

ValueTree TimelineData::getKeyframe(ValueTree sequence, const var& position) {
	const int wantedPosition = position;
	std::lock_guard<std::recursive_mutex> lock(treeMutex);

	const int numKeyframes = getNumKeyframes(sequence);
	for (int i = 0; i < numKeyframes; i++) {
		ValueTree keyframe = getKeyframe(sequence, i);
		const int keyframePosition = getKeyframePosition(keyframe);
		if (wantedPosition == keyframePosition) {
			return keyframe;
		}
	}

	return ValueTree();
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

	ValueTree interpolatedValue = interpolator.calculateInterpolatedState(sequence, keyframePosition).first;
	getKeyframeValue(keyframe).copyPropertiesFrom(interpolatedValue, nullptr);

	addKeyframeUnchecked(sequence, keyframe);
	return keyframe;
}

// adds a keyframe to the keyframes array of a sequence
// returns the keyframe again
// doesn't perform any checking (you should use addKeyframe(ValueTree))
// keyframe gets inserted at a sorted position
ValueTree TimelineData::addKeyframeUnchecked(ValueTree sequence, ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	const int numKeyframes = getNumKeyframes(sequence);
	int sortedPosition = 0;
	for (; sortedPosition < numKeyframes; sortedPosition++) {
		ValueTree otherKeyframe = getKeyframe(sequence, sortedPosition);
		if (compareKeyframes(keyframe, otherKeyframe) < 0) {
			// break if the new keyframe comes before the currently checked one
			break;
		}
		// if the new keyframe comes last sortedPosition will contain numKeyframes
		// and the new keyframe will be added at the end
	}
	getKeyframesArray(sequence).addChild(keyframe, sortedPosition, &undoManager);
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
	return keyframe.getOrCreateChildWithName(treeId::keyframeValue, nullptr);
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


// returns the index of the keyframe in it's keyframesArray
int TimelineData::getKeyframeIndex(ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return keyframe.getParent().indexOf(keyframe);
}

// returns the sequence the keyframe belongs to
// of course the keyframe must already be added to one
ValueTree TimelineData::getKeyframeParentSequence(ValueTree keyframe) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return keyframe.getParent().getParent();
}

// comparator function for keyframes in the keyframes array of a sequence
int TimelineData::compareKeyframes(const ValueTree& first, const ValueTree& second) {
		int firstPosition = getKeyframePosition(first);
		int secondPosition = getKeyframePosition(second);
		return firstPosition - secondPosition;
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
		setValueBoolState(valueData, true, false);
	} else if (valueType == "float") {
		setValueFloatX(valueData, 1., false);
	} else if (valueType == "vec2") {
		setValueVec2X(valueData, 1., false);
		setValueVec2Y(valueData, 1., false);
	} else if (valueType == "vec3") {
		setValueVec3X(valueData, 1., false);
		setValueVec3Y(valueData, 1., false);
		setValueVec3Z(valueData, 1., false);
	} else if (valueType == "vec4") {
		setValueVec4X(valueData, 1., false);
		setValueVec4Y(valueData, 1., false);
		setValueVec4Z(valueData, 1., false);
		setValueVec4W(valueData, 1., false);
	} else if (valueType == "color") {
		setValueColorR(valueData, .18, false);
		setValueColorG(valueData, .18, false);
		setValueColorB(valueData, .18, false);
	} else if (valueType == "quat") {
		// (0, 0, 0, 1) is the identity rotation
		setValueQuatX(valueData, 0., false);
		setValueQuatY(valueData, 0., false);
		setValueQuatZ(valueData, 0., false);
		setValueQuatW(valueData, 1., false);
	} else {
		return false;
	}
	return true;
}

// returns the value as a string
// it uses glsl-style initalizer
String TimelineData::getValueAsString(ValueTree value) {
	if (isValueFloat(value)) {
		return getValueFloatX(value);
	} else if (isValueVec2(value)) {
		return "vec2(" + getValueVec2X(value).toString() + ", " + getValueVec2Y(value).toString() + ")";
	} else if (isValueVec3(value)) {
		return "vec3(" + getValueVec3X(value).toString() + ", " + getValueVec3Y(value).toString() + ", " + getValueVec3Z(value).toString() + ")";
	} else if (isValueVec4(value)) {
		return "vec4(" + getValueVec4X(value).toString() + ", " + getValueVec4Y(value).toString() + ", " + getValueVec4Z(value).toString() + ", " + getValueVec4W(value).toString() + ")";
	} else if (isValueColor(value)) {
		return "vec3(" + getValueColorR(value).toString() + ", " + getValueColorG(value).toString() + ", " + getValueColorB(value).toString() + ")";
	} else if (isValueQuat(value)) {
		return "vec4(" + getValueQuatX(value).toString() + ", " + getValueQuatY(value).toString() + ", " + getValueQuatZ(value).toString() + ", " + getValueQuatW(value).toString() + ")";
	} else if (isValueBool(value)) {
		return getBoolFromValue(value) ? "true" : "false";
	}
	return String();
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

// checks if the value is of type vec4
bool TimelineData::isValueVec4(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueVec4X);
}

// checks if the value is of type color
bool TimelineData::isValueColor(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueColorR);
}

// checks if the value is of type quat
bool TimelineData::isValueQuat(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.hasProperty(treeId::valueQuatX);
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

// get vec4X of a vec4 value
var TimelineData::getValueVec4X(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec4X);
}

// get vec4Y of a vec4 value
var TimelineData::getValueVec4Y(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec4Y);
}

// get vec4Z of a vec4 value
var TimelineData::getValueVec4Z(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec4Z);
}

// get vec4W of a vec4 value
var TimelineData::getValueVec4W(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueVec4W);
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

// get quatX of a quat value
var TimelineData::getValueQuatX(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueQuatX);
}

// get quatY of a quat value
var TimelineData::getValueQuatY(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueQuatY);
}

// get quatZ of a quat value
var TimelineData::getValueQuatZ(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueQuatZ);
}

// get quatW of a quat value
var TimelineData::getValueQuatW(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getProperty(treeId::valueQuatW);
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

// get vec4X of a vec4 value as a juce::Value
Value TimelineData::getValueVec4XAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec4X, &undoManager);
}

// get vec4Y of a vec4 value as a juce::Value
Value TimelineData::getValueVec4YAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec4Y, &undoManager);
}

// get vec4Z of a vec4 value as a juce::Value
Value TimelineData::getValueVec4ZAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec4Z, &undoManager);
}

// get vec4W of a vec4 value as a juce::Value
Value TimelineData::getValueVec4WAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueVec4W, &undoManager);
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

// get quatX of a quat value as a juce::Value
Value TimelineData::getValueQuatXAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueQuatX, &undoManager);
}

// get quatY of a quat value as a juce::Value
Value TimelineData::getValueQuatYAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueQuatY, &undoManager);
}

// get quatZ of a quat value as a juce::Value
Value TimelineData::getValueQuatZAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueQuatZ, &undoManager);
}

// get quatW of a quat value as a juce::Value
Value TimelineData::getValueQuatWAsValue(ValueTree value) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	return value.getPropertyAsValue(treeId::valueQuatW, &undoManager);
}


// TODO: some checking maybe
// set boolState of a bool value
void TimelineData::setValueBoolState(ValueTree value, var boolState, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueBoolState, boolState, useUndoManager ? &undoManager : nullptr);
}

// set floatX of a float value
void TimelineData::setValueFloatX(ValueTree value, var floatX, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueFloatX, floatX, useUndoManager ? &undoManager : nullptr);
}

// set vec2X of a vec2 value
void TimelineData::setValueVec2X(ValueTree value, var vec2X, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec2X, vec2X, useUndoManager ? &undoManager : nullptr);
}

// set vec2Y of a vec2 value
void TimelineData::setValueVec2Y(ValueTree value, var vec2Y, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec2Y, vec2Y, useUndoManager ? &undoManager : nullptr);
}

// set vec3X of a vec3 value
void TimelineData::setValueVec3X(ValueTree value, var vec3X, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec3X, vec3X, useUndoManager ? &undoManager : nullptr);
}

// set vec3Y of a vec3 value
void TimelineData::setValueVec3Y(ValueTree value, var vec3Y, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec3Y, vec3Y, useUndoManager ? &undoManager : nullptr);
}

// set vec3Z of a vec3 value
void TimelineData::setValueVec3Z(ValueTree value, var vec3Z, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec3Z, vec3Z, useUndoManager ? &undoManager : nullptr);
}

// set vec4X of a vec4 value
void TimelineData::setValueVec4X(ValueTree value, var vec4X, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec4X, vec4X, useUndoManager ? &undoManager : nullptr);
}

// set vec4Y of a vec4 value
void TimelineData::setValueVec4Y(ValueTree value, var vec4Y, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec4Y, vec4Y, useUndoManager ? &undoManager : nullptr);
}

// set vec4Z of a vec4 value
void TimelineData::setValueVec4Z(ValueTree value, var vec4Z, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec4Z, vec4Z, useUndoManager ? &undoManager : nullptr);
}

// set vec4W of a vec4 value
void TimelineData::setValueVec4W(ValueTree value, var vec4W, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueVec4W, vec4W, useUndoManager ? &undoManager : nullptr);
}

// set colorR of a color value
void TimelineData::setValueColorR(ValueTree value, var colorR, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueColorR, colorR, useUndoManager ? &undoManager : nullptr);
}

// set colorG of a color value
void TimelineData::setValueColorG(ValueTree value, var colorG, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueColorG, colorG, useUndoManager ? &undoManager : nullptr);
}

// set colorB of a color value
void TimelineData::setValueColorB(ValueTree value, var colorB, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueColorB, colorB, useUndoManager ? &undoManager : nullptr);
}

// set quatX of a quat value
void TimelineData::setValueQuatX(ValueTree value, var quatX, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueQuatX, quatX, useUndoManager ? &undoManager : nullptr);
}

// set quatY of a quat value
void TimelineData::setValueQuatY(ValueTree value, var quatY, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueQuatY, quatY, useUndoManager ? &undoManager : nullptr);
}

// set quatZ of a quat value
void TimelineData::setValueQuatZ(ValueTree value, var quatZ, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueQuatZ, quatZ, useUndoManager ? &undoManager : nullptr);
}

// set quatW of a quat value
void TimelineData::setValueQuatW(ValueTree value, var quatW, bool useUndoManager) {
	std::lock_guard<std::recursive_mutex> lock(treeMutex);
	value.setProperty(treeId::valueQuatW, quatW, useUndoManager ? &undoManager : nullptr);
}



// returns a bool value as a bool
bool TimelineData::getBoolFromValue(ValueTree value) {
	const bool boolState = getValueBoolState(value);
	return boolState;
}

// returns a float value as a float
float TimelineData::getFloatFromValue(ValueTree value) {
	const float floatX = getValueFloatX(value);
	return floatX;
}

// returns a vec2 value as a glm::vec2
glm::vec2 TimelineData::getVec2FromValue(ValueTree value) {
	const float vec2X = getValueVec2X(value);
	const float vec2Y = getValueVec2Y(value);
	return glm::vec2(vec2X, vec2Y);
}

// returns a vec3 value as a glm::vec3
glm::vec3 TimelineData::getVec3FromValue(ValueTree value) {
	const float vec3X = getValueVec3X(value);
	const float vec3Y = getValueVec3Y(value);
	const float vec3Z = getValueVec3Z(value);
	return glm::vec3(vec3X, vec3Y, vec3Z);
}

// returns a vec4 value as a glm::vec4
glm::vec4 TimelineData::getVec4FromValue(ValueTree value) {
	const float vec4X = getValueVec4X(value);
	const float vec4Y = getValueVec4Y(value);
	const float vec4Z = getValueVec4Z(value);
	const float vec4W = getValueVec4W(value);
	return glm::vec4(vec4X, vec4Y, vec4Z, vec4W);
}

// returns a color value as a glm::vec3
glm::vec3 TimelineData::getColorFromValue(ValueTree value) {
	const float colorR = getValueColorR(value);
	const float colorG = getValueColorG(value);
	const float colorB = getValueColorB(value);
	return glm::vec3(colorR, colorG, colorB);
}

// returns a quat value as a glm::quat
glm::quat TimelineData::getQuatFromValue(ValueTree value) {
	const float quatX = getValueQuatX(value);
	const float quatY = getValueQuatY(value);
	const float quatZ = getValueQuatZ(value);
	const float quatW = getValueQuatW(value);
	return glm::quat(quatW, quatX, quatY, quatZ);
}


// sets the contents of a float value to the numbers from a float
void TimelineData::setFloatToValue(ValueTree value, float scalar, bool useUndoManager) {
	jassert(isValueFloat(value) || getNumValueProperties(value) == 0);
	setValueFloatX(value, scalar, useUndoManager);
}

// sets the contents of a vec2 value to the numbers from a glm::vec2
void TimelineData::setVec2ToValue(ValueTree value, glm::vec2 vector, bool useUndoManager) {
	jassert(isValueVec2(value) || getNumValueProperties(value) == 0);
	setValueVec2X(value, vector.x, useUndoManager);
	setValueVec2Y(value, vector.y, useUndoManager);
}

// sets the contents of a vec3 value to the numbers from a glm::vec3
void TimelineData::setVec3ToValue(ValueTree value, glm::vec3 vector, bool useUndoManager) {
	jassert(isValueVec3(value) || getNumValueProperties(value) == 0);
	setValueVec3X(value, vector.x, useUndoManager);
	setValueVec3Y(value, vector.y, useUndoManager);
	setValueVec3Z(value, vector.z, useUndoManager);
}

// sets the contents of a vec4 value to the numbers from a glm::vec4
void TimelineData::setVec4ToValue(ValueTree value, glm::vec4 vector, bool useUndoManager) {
	jassert(isValueVec4(value) || getNumValueProperties(value) == 0);
	setValueVec4X(value, vector.x, useUndoManager);
	setValueVec4Y(value, vector.y, useUndoManager);
	setValueVec4Z(value, vector.z, useUndoManager);
	setValueVec4W(value, vector.w, useUndoManager);
}

// sets the contents of a color value to the numbers from a glm::vec2
void TimelineData::setColorToValue(ValueTree value, glm::vec3 vector, bool useUndoManager) {
	jassert(isValueColor(value) || getNumValueProperties(value) == 0);
	setValueColorR(value, vector.r, useUndoManager);
	setValueColorG(value, vector.g, useUndoManager);
	setValueColorB(value, vector.b, useUndoManager);
}

// sets the contents of a quat value to the numbers from a glm::quat
void TimelineData::setQuatToValue(ValueTree value, glm::quat vector, bool useUndoManager) {
	jassert(isValueQuat(value) || getNumValueProperties(value) == 0);
	setValueQuatX(value, vector.x, useUndoManager);
	setValueQuatY(value, vector.y, useUndoManager);
	setValueQuatZ(value, vector.z, useUndoManager);
	setValueQuatW(value, vector.w, useUndoManager);
}


// mixes two values using glm::mix
// does different stuff for bool, float, vec2, etc...
// t should be between 0 and 1
ValueTree TimelineData::mixValues(ValueTree value1, ValueTree value2, const float t) {
	if (isValueFloat(value1)) {
		jassert(isValueFloat(value2));
		const float floatX1 = getValueFloatX(value1);
		const float floatX2 = getValueFloatX(value2);
		const float floatInterpolated = glm::mix(floatX1, floatX2, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setValueFloatX(interpolatedValue, floatInterpolated, false);
		return interpolatedValue;
	} else if (isValueVec2(value1)) {
		jassert(isValueVec2(value2));
		const glm::vec2 vec21 = getVec2FromValue(value1);
		const glm::vec2 vec22 = getVec2FromValue(value2);
		const glm::vec2 vec2Interpolated = glm::mix(vec21, vec22, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setVec2ToValue(interpolatedValue, vec2Interpolated, false);
		return interpolatedValue;
	} else if (isValueVec3(value1)) {
		jassert(isValueVec3(value2));
		const glm::vec3 vec31 = getVec3FromValue(value1);
		const glm::vec3 vec32 = getVec3FromValue(value2);
		const glm::vec3 vec3Interpolated = glm::mix(vec31, vec32, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setVec3ToValue(interpolatedValue, vec3Interpolated, false);
		return interpolatedValue;
	} else if (isValueVec4(value1)) {
		jassert(isValueVec4(value2));
		const glm::vec4 vec41 = getVec4FromValue(value1);
		const glm::vec4 vec42 = getVec4FromValue(value2);
		const glm::vec4 vec4Interpolated = glm::mix(vec41, vec42, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setVec4ToValue(interpolatedValue, vec4Interpolated, false);
		return interpolatedValue;
	} else if (isValueColor(value1)) {
		jassert(isValueColor(value2));
		const glm::vec3 color1 = getColorFromValue(value1);
		const glm::vec3 color2 = getColorFromValue(value2);
		// TODO: do better (maybe in Hcl?) interpolation
		const glm::vec3 colorInterpolated = glm::mix(color1, color2, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setColorToValue(interpolatedValue, colorInterpolated, false);
		return interpolatedValue;
	} else if (isValueQuat(value1)) {
		jassert(isValueQuat(value2));
		const glm::quat quat1 = getQuatFromValue(value1);
		const glm::quat quat2 = getQuatFromValue(value2);
		const glm::quat quatInterpolated = glm::slerp(quat1, quat2, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setQuatToValue(interpolatedValue, quatInterpolated, false);
		return interpolatedValue;
	} else if (isValueBool(value1)) {
		jassert(isValueBool(value2));
		jassertfalse; // bool mixing doesn't make any sense
		return ValueTree();
	}

	jassertfalse;
	return ValueTree();
}

// returns the value of a centripetal Catmull-Rom spline for points P0, P1, P2 and P3 at parameter t
// t should be between 0 and 1
// if valueP0 or valueP3 are invalid (at start or end of sequence), some good values will be choosen
ValueTree TimelineData::calculateCcrSplineForValues(ValueTree valueP0, ValueTree valueP1, ValueTree valueP2, ValueTree valueP3, const float t) {
	if (isValueVec3(valueP1)) {
		jassert(isValueVec3(valueP2));

		const glm::vec3 P1 = getVec3FromValue(valueP1);
		const glm::vec3 P2 = getVec3FromValue(valueP2);

		// if values are invalid, mirror P2/P1 at P1/P2
		const glm::vec3 P0 = valueP0.isValid() ? getVec3FromValue(valueP0) : P1 - (P2 - P1);
		const glm::vec3 P3 = valueP3.isValid() ? getVec3FromValue(valueP3) : P2 - (P1 - P2);

		const glm::vec3 interpolatedP = CentripetalCatmullRomSpline(P0, P1, P2, P3, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setVec3ToValue(interpolatedValue, interpolatedP, false);
		return interpolatedValue;
	} else if (isValueQuat(valueP1)) {
		jassert(isValueQuat(valueP2));

		const glm::quat P1 = getQuatFromValue(valueP1);
		const glm::quat P2 = getQuatFromValue(valueP2);

		// if values are invalid, mirror P2/P1 at P1/P2
		glm::quat P0;
		glm::quat P3;
		if (valueP0.isValid()) {
			P0 = getQuatFromValue(valueP0);
		} else {
			P0 = quat_mirror(P2, P1);
		}
		if (valueP3.isValid()) {
			P3 = getQuatFromValue(valueP3);
		} else {
			P3 = quat_mirror(P1, P2);
		}

		const glm::quat interpolatedP = CentripetalCatmullRomSpline(P0, P1, P2, P3, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setQuatToValue(interpolatedValue, interpolatedP, false);
		return interpolatedValue;
	} else if (isValueVec2(valueP1)) {
		jassert(isValueVec2(valueP2));

		const glm::vec2 P1 = getVec2FromValue(valueP1);
		const glm::vec2 P2 = getVec2FromValue(valueP2);

		// if values are invalid, mirror P2/P1 at P1/P2
		const glm::vec2 P0 = valueP0.isValid() ? getVec2FromValue(valueP0) : P1 - (P2 - P1);
		const glm::vec2 P3 = valueP3.isValid() ? getVec2FromValue(valueP3) : P2 - (P1 - P2);

		const glm::vec2 interpolatedP = CentripetalCatmullRomSpline(P0, P1, P2, P3, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setVec2ToValue(interpolatedValue, interpolatedP, false);
		return interpolatedValue;
	} else if (isValueFloat(valueP1)) {
		jassert(isValueFloat(valueP2));

		const float P1 = getFloatFromValue(valueP1);
		const float P2 = getFloatFromValue(valueP2);

		// if values are invalid, mirror P2/P1 at P1/P2
		const float P0 = valueP0.isValid() ? getFloatFromValue(valueP0) : P1 - (P2 - P1);
		const float P3 = valueP3.isValid() ? getFloatFromValue(valueP3) : P2 - (P1 - P2);

		const float interpolatedP = CentripetalCatmullRomSpline(P0, P1, P2, P3, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setFloatToValue(interpolatedValue, interpolatedP, false);
		return interpolatedValue;
	} else if (isValueVec4(valueP1)) {
		jassert(isValueVec4(valueP2));

		const glm::vec4 P1 = getVec4FromValue(valueP1);
		const glm::vec4 P2 = getVec4FromValue(valueP2);

		// if values are invalid, mirror P2/P1 at P1/P2
		const glm::vec4 P0 = valueP0.isValid() ? getVec4FromValue(valueP0) : P1 - (P2 - P1);
		const glm::vec4 P3 = valueP3.isValid() ? getVec4FromValue(valueP3) : P2 - (P1 - P2);

		const glm::vec4 interpolatedP = CentripetalCatmullRomSpline(P0, P1, P2, P3, t);

		ValueTree interpolatedValue(treeId::interpolatedValue);
		setVec4ToValue(interpolatedValue, interpolatedP, false);
		return interpolatedValue;
	}

	jassertfalse;
	return ValueTree();
}

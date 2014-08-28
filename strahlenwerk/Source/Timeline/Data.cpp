#include "Data.h"
#include "TreeIdentifiers.h"

// comparator for keyframes in the keyframes array of a sequence
struct KeyframesComparator {
	int compareElements (const ValueTree& first, const ValueTree& second) const {
		int firstPosition = first.getProperty(treeId::keyframePosition);
		int secondPosition = second.getProperty(treeId::keyframePosition);
		return firstPosition - secondPosition;
	}
};
static KeyframesComparator keyframesComparator;

Data::Data() :
	currentTime(40),
	valueTree(treeId::timelineTree)
{
	for (int i = 0; i < 4; i++) {
		addScene(var(i+1), var(300 * i), var(50 * (i + 1)), var(String(i) + String(41 * i) + ".glsl"));
	}

	for (int i = 0; i < 30; i++) {
		addUniform(var("uniform" + String(i) + String(97 * i)), var(i%2 == 0 ? "color" : "vec3"));

		for (int j = 0; j <= (37*i % 4); j++) {
			ValueTree sequence(treeId::sequence);
			const int absoluteStart = (j + (97*i % 10)) * 100;
			setSequencePropertiesForAbsoluteStart(sequence, absoluteStart);
			sequence.setProperty(treeId::sequenceDuration, var(50), nullptr);
			sequence.setProperty(treeId::sequenceInterpolation, var("linear"), nullptr);
			initializeKeyframesArray(sequence);
			addKeyframe(sequence, var((j+1) * 10));
			addSequence(getUniformsArray().getChild(i), sequence);
		}
	}
}

// retrieves the scenes array
ValueTree Data::getScenesArray() {
	return valueTree.getOrCreateChildWithName(treeId::scenesArray, &undoManager);
}

// adds a scene to the scenes array at a given position
// returns whether the scene was added
// if the ValueTree is not a treeId::scene, it won't get added
// position defaults to -1 (append to end)
bool Data::addScene(ValueTree scene, int position) {
	bool isScene = scene.hasType(treeId::scene);
	isScene &= scene.hasProperty(treeId::sceneId);
	isScene &= scene.hasProperty(treeId::sceneStart);
	isScene &= scene.hasProperty(treeId::sceneDuration);
	isScene &= scene.hasProperty(treeId::sceneShaderSource);

	if (isScene) {
		getScenesArray().addChild(scene, position, &undoManager);
	}
	return isScene;
}

// adds a scene with the given vars at position
// the scene will always be added
// returns always true
// position defaults to -1 (append to end)
bool Data::addScene(var id, var start, var duration, var shaderSource, int position) {
	ValueTree scene(treeId::scene);
	scene.setProperty(treeId::sceneId, id, nullptr);
	scene.setProperty(treeId::sceneStart, start, nullptr);
	scene.setProperty(treeId::sceneDuration, duration, nullptr);
	scene.setProperty(treeId::sceneShaderSource, shaderSource, nullptr);
	getScenesArray().addChild(scene, position, &undoManager);
	return true;
}

// finds the the end time of the last scene
int Data::getLastSceneEndTime() {
	ValueTree scenesArray = getScenesArray();
	const int numChildren = scenesArray.getNumChildren();
	int maxEndTime = 0;

	for (int i = 0; i < numChildren; i++) {
		ValueTree scene = scenesArray.getChild(i);
		const int start = scene.getProperty(treeId::sceneStart);
		const int duration = scene.getProperty(treeId::sceneDuration);
		const int end = start + duration;
		maxEndTime = jmax(maxEndTime, end);
	}

	return maxEndTime;
}

// returns the active scene for a timepoint
ValueTree Data::getSceneForTime(const int time) {
	ValueTree scenesArray = getScenesArray();
	const int numScenes = scenesArray.getNumChildren();
	/*
	// matches if the time is inside the scene and also if after the end
	int smallestDistance = INT_MAX;
	int bestScene = 0;

	for (int i = 0; i < numScenes; i++) {
		ValueTree scene = scenesArray.getChild(i);
		const int start = scene.getProperty(treeId::sceneStart);
		const int distance = time - start;
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
		ValueTree scene = scenesArray.getChild(i);
		const int start = scene.getProperty(treeId::sceneStart);
		const int duration = scene.getProperty(treeId::sceneDuration);
		const int distance = time - start;
		if (isPositiveAndBelow(distance, duration)) {
			return scene;
		}
	}
	// return invalid tree
	return ValueTree();
	// */
}

// returns a scene Id that is currently not used
int Data::getNewSceneId() {
	int biggestId = 99; // some room for $stuff
	ValueTree scenesArray = getScenesArray();
	const int numScenes = scenesArray.getNumChildren();

	for (int i = 0; i < numScenes; i++) {
		ValueTree scene = scenesArray.getChild(i);
		const int currentId = scene.getProperty(treeId::sceneId);
		biggestId = jmax(biggestId, currentId);
	}
	return biggestId + 1;
}

// retrieves the uniforms array
ValueTree Data::getUniformsArray() {
	return valueTree.getOrCreateChildWithName(treeId::uniformsArray, &undoManager);
}

// adds a uniform to the uniforms array at a given position
// returns whether the uniform was added
// if the ValueTree is not a treeId::uniform, it won't get added
// position defaults to -1 (append to end)
bool Data::addUniform(ValueTree uniform, int position) {
	bool isUniform = uniform.hasType(treeId::uniform);
	isUniform &= uniform.hasProperty(treeId::uniformName);
	isUniform &= uniform.hasProperty(treeId::uniformType);
	isUniform &= uniform.getChildWithName(treeId::uniformStandardValue).isValid();

	if (isUniform) {
		getUniformsArray().addChild(uniform, position, &undoManager);
	}
	return isUniform;
}

// adds a uniform with the given vars at position
// the uniform will always be added
// returns always true
// position defaults to -1 (append to end)
bool Data::addUniform(var name, var type, int position) {
	ValueTree uniform(treeId::uniform);
	uniform.setProperty(treeId::uniformName, name, nullptr);
	uniform.setProperty(treeId::uniformType, type, nullptr);
	initializeValue(uniform.getOrCreateChildWithName(treeId::uniformStandardValue, nullptr), type);
	getUniformsArray().addChild(uniform, position, &undoManager);
	return true;
}

ValueTree Data::getUniform(const var& name) {
	return getUniformsArray().getChildWithProperty(treeId::uniformName, name);
}

// retrieves the sequences array for a given uniform
ValueTree Data::getSequencesArray(ValueTree uniform) {
	return uniform.getOrCreateChildWithName(treeId::sequencesArray, &undoManager);
}

// adds a sequence to the sequences array of a uniform at a given position
// returns whether the sequence was added
// if the ValueTree is not a treeId::sequence, it won't get added
// position defaults to -1 (append to end)
bool Data::addSequence(ValueTree uniform, ValueTree sequence, int position) {
	bool isSequence = sequence.hasType(treeId::sequence);
	isSequence &= sequence.hasProperty(treeId::sequenceSceneId);
	isSequence &= sequence.hasProperty(treeId::sequenceStart);
	isSequence &= sequence.hasProperty(treeId::sequenceDuration);
	isSequence &= sequence.hasProperty(treeId::sequenceInterpolation);
	isSequence &= sequence.getChildWithName(treeId::keyframesArray).getNumChildren() >= 2;

	if (isSequence) {
		getSequencesArray(uniform).addChild(sequence, position, &undoManager);
	}
	return isSequence;
}

// adds a sequence with the given vars to a uniform at position
// the sequence will always be added
// returns always true
// position defaults to -1 (append to end)
bool Data::addSequence(ValueTree uniform, var sceneId, var start, var duration, var interpolation, int position) {
	ValueTree sequence(treeId::sequence);
	sequence.setProperty(treeId::sequenceSceneId, sceneId, nullptr);
	sequence.setProperty(treeId::sequenceStart, start, nullptr);
	sequence.setProperty(treeId::sequenceDuration, duration, nullptr);
	sequence.setProperty(treeId::sequenceInterpolation, interpolation, nullptr);
	initializeKeyframesArray(sequence);
	getSequencesArray(uniform).addChild(sequence, position, &undoManager);
	return true;
}

// sets the sceneId and relative start time of a sequence for a given absolute start time
// return true when the scene for this sequence is a new one, false otherwise
bool Data::setSequencePropertiesForAbsoluteStart(ValueTree sequence, int absoluteStart) {
			ValueTree sceneForSequence = getSceneForTime(absoluteStart);
			var sceneId = sceneForSequence.getProperty(treeId::sceneId);

			const int sceneStart = sceneForSequence.getProperty(treeId::sceneStart);
			var relativeStart = absoluteStart - sceneStart;

			sequence.setProperty(treeId::sequenceStart, relativeStart, &undoManager);

			var oldSceneId = sequence.getProperty(treeId::sequenceSceneId);
			bool sceneIdChanged = !oldSceneId.equals(sceneId);
			if (sceneIdChanged) {
				sequence.setProperty(treeId::sequenceSceneId, sceneId, &undoManager);
			}
			return sceneIdChanged;
}

// returns the keyframes array for a sequence
ValueTree Data::getKeyframesArray(ValueTree sequence) {
	return sequence.getOrCreateChildWithName(treeId::keyframesArray, &undoManager);
}

// initialize the keyframesArray of the sequence
// creates the start and end keyframe
// return whether the sequence was properly initialized
bool Data::initializeKeyframesArray(ValueTree sequence) {
	if (!sequence.hasProperty(treeId::sequenceDuration)) {
		// sequence is not initialized
		return false;
	}

	var relativeEndTime = sequence.getProperty(treeId::sequenceDuration);
	addKeyframe(sequence, var(0));
	addKeyframe(sequence, relativeEndTime);

	return true;
}

// adds a keyframe to the keyframes array of a sequence at the right position
// returns whether the sequence was added
// if the ValueTree is not a treeId::keyframe, it won't get added
bool Data::addKeyframe(ValueTree sequence, ValueTree keyframe) {
	bool isKeyframe = keyframe.hasType(treeId::keyframe);
	isKeyframe &= keyframe.hasProperty(treeId::keyframePosition);

	var uniformType = sequence.getParent().getParent().getProperty(treeId::uniformType);
	var keyframeValueType = keyframe.getChildWithName(treeId::keyframeValue).getProperty(treeId::valueType);
	isKeyframe &= uniformType.equals(keyframeValueType);

	if (isKeyframe) {
		ValueTree keyframesArray = getKeyframesArray(sequence);
		keyframesArray.addChild(keyframe, -1, &undoManager);
		keyframesArray.sort<KeyframesComparator>(keyframesComparator, &undoManager, true);
	}
	return isKeyframe;
}

// adds a keyframe with the given time position to a sequence at the right position
// the keyframe will always be added
// returns always true
bool Data::addKeyframe(ValueTree sequence, var keyframePosition) {
	ValueTree keyframe(treeId::keyframe);
	keyframe.setProperty(treeId::keyframePosition, keyframePosition, nullptr);

	ValueTree keyframeValue(treeId::keyframeValue);
	var uniformType = sequence.getParent().getParent().getProperty(treeId::uniformType);
	initializeValue(keyframeValue, uniformType);
	keyframe.addChild(keyframeValue, -1, nullptr);

	ValueTree keyframesArray = getKeyframesArray(sequence);
	keyframesArray.addChild(keyframe, -1, &undoManager);
	keyframesArray.sort<KeyframesComparator>(keyframesComparator, &undoManager, true);
	return true;
}

// initialize a value with the specified type inside valueData
// return false when valueData was already initialized or the type is unknown
bool Data::initializeValue(ValueTree valueData, var type) {
	if (valueData.hasProperty(treeId::valueType)) {
		// value is already populated
		return false;
	}
	valueData.setProperty(treeId::valueType, type, &undoManager);

	String valueType = type;
	if (valueType.equalsIgnoreCase("bool")) {
		valueData.getOrCreateChildWithName(treeId::valueBool, &undoManager)
			.setProperty(treeId::valueBoolState, var(false), &undoManager);
	} else if (valueType.equalsIgnoreCase("float")) {
		valueData.getOrCreateChildWithName(treeId::valueFloat, &undoManager)
			.setProperty(treeId::valueFloatX, var(0.), &undoManager);
	} else if (valueType.equalsIgnoreCase("vec2")) {
		valueData.getOrCreateChildWithName(treeId::valueVec2, &undoManager)
			.setProperty(treeId::valueVec2X, var(0.), &undoManager)
			.setProperty(treeId::valueVec2Y, var(0.), &undoManager);
	} else if (valueType.equalsIgnoreCase("vec3")) {
		valueData.getOrCreateChildWithName(treeId::valueVec3, &undoManager)
			.setProperty(treeId::valueVec3X, var(0.), &undoManager)
			.setProperty(treeId::valueVec3Y, var(0.), &undoManager)
			.setProperty(treeId::valueVec3Z, var(0.), &undoManager);
	} else if (valueType.equalsIgnoreCase("color")) {
		valueData.getOrCreateChildWithName(treeId::valueColor, &undoManager)
			.setProperty(treeId::valueColorR, var(0.), &undoManager)
			.setProperty(treeId::valueColorG, var(0.), &undoManager)
			.setProperty(treeId::valueColorB, var(0.), &undoManager);
	} else {
		return false;
	}
	return true;
}

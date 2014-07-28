#include "Data.h"
#include "TreeIdentifiers.h"

Data::Data() :
	valueTree(treeId::timelineTree)
{
	for (int i = 0; i < 4; i++) {
		addScene(var(i), var(300 * i), var(50 * (i + 1)), var(String(i) + String(41 * i) + ".glsl"));
	}

	for (int i = 0; i < 30; i++) {
		addUniform(var("uniform" + String(i) + String(97 * i)), var(i%2 == 0 ? "color" : "float"));

		for (int j = 0; j <= (37*i % 4); j++) {
			const int absoluteStart = (j + (97*i % 10)) * 100;
			var duration = var(50);
			ValueTree sceneForSequence = getSceneForTime(absoluteStart);
			const int sceneStart = sceneForSequence.getProperty(treeId::sceneStart);
			var relativeStart = absoluteStart - sceneStart;
			var sceneId = sceneForSequence.getProperty(treeId::sceneId);
			var interpolation = var("linear");
			addSequence(getUniformsArray().getChild(i), sceneId, relativeStart, duration, interpolation);
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

// returns the active or last active scene for a timepoint
ValueTree Data::getSceneForTime(const int time) {
	ValueTree scenesArray = getScenesArray();
	const int numScenes = scenesArray.getNumChildren();
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
	getUniformsArray().addChild(uniform, position, &undoManager);
	return true;
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
	getSequencesArray(uniform).addChild(sequence, position, &undoManager);
	return true;
}

#include "Data.h"
#include "TreeIdentifiers.h"

Data::Data() :
	valueTree(treeId::timelineTree)
{
	for (int i = 0; i < 4; i++) {
		addScene(var(i), var(60 * i), var(10 * i + 10), var("glsl" + String(i)));
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
		getScenesArray().addChild(scene, -1, &undoManager);
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
	int numChildren = scenesArray.getNumChildren();
	int maxEndTime = 0;

	for (int i = 0; i < numChildren; i++) {
		ValueTree scene = scenesArray.getChild(i);
		int start = scene.getProperty(treeId::sceneStart);
		int duration = scene.getProperty(treeId::sceneDuration);
		int end = start + duration;
		maxEndTime = jmax(maxEndTime, end);
	}

	return maxEndTime;
}

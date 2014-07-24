#include "Data.h"
#include "TreeIdentifiers.h"

Data::Data() :
	valueTree(ttId::timelineTree)
{
	for (int i = 0; i < 4; i++) {
		addScene(var(i), var(60 * i), var(10 * i + 10), var("glsl" + String(i)));
	}
}

// retrieves the scenes array
ValueTree Data::getScenesArray() {
	return valueTree.getOrCreateChildWithName(ttId::scenesArray, &undoManager);
}

// adds a scene to the scenes array at a given position
// returns whether the scene was added
// if the ValueTree is not a ttId::scene, it won't get added
// position defaults to -1 (append to end)
bool Data::addScene(ValueTree scene, int position) {
	bool isScene = scene.hasType(ttId::scene);
	isScene &= scene.hasProperty(ttId::sceneId);
	isScene &= scene.hasProperty(ttId::sceneStart);
	isScene &= scene.hasProperty(ttId::sceneDuration);
	isScene &= scene.hasProperty(ttId::sceneShaderSource);

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
	ValueTree scene(ttId::scene);
	scene.setProperty(ttId::sceneId, id, nullptr);
	scene.setProperty(ttId::sceneStart, start, nullptr);
	scene.setProperty(ttId::sceneDuration, duration, nullptr);
	scene.setProperty(ttId::sceneShaderSource, shaderSource, nullptr);
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
		int start = scene.getProperty(ttId::sceneStart);
		int duration = scene.getProperty(ttId::sceneDuration);
		int end = start + duration;
		maxEndTime = jmax(maxEndTime, end);
	}

	return maxEndTime;
}

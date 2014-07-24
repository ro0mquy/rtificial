#include "Data.h"
#include "TreeIdentifiers.h"

Data::Data() :
	valueTree(ttId::timelineTree)
{
	ValueTree scenesArray(ttId::scenesArray);
	for (int i = 0; i < 4; i++) {
		ValueTree scene(ttId::scene);
		scene.setProperty(ttId::sceneId, var(i), nullptr);
		scene.setProperty(ttId::sceneStart, var(60 * i), nullptr);
		scene.setProperty(ttId::sceneDuration, var(60 * i + 40), nullptr);
		scene.setProperty(ttId::sceneShaderSource, var(String("glsl") + String(i)), nullptr);
		scenesArray.addChild(scene, -1, nullptr);
	}
	valueTree.addChild(scenesArray, -1, nullptr);
}

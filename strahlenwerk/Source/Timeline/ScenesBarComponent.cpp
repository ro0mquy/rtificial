#include "ScenesBarComponent.h"

#include "TreeIdentifiers.h"
#include "../RtificialLookAndFeel.h"
#include "TimelineData.h"
#include "SceneComponent.h"

ScenesBarComponent::ScenesBarComponent() :
	data(TimelineData::getTimelineData())
{
	updateSceneComponents();
}

ScenesBarComponent::~ScenesBarComponent() = default;

void ScenesBarComponent::updateSize() {
	const int width = jmax(data.getLastSceneEndTime() + 20, getParentWidth());
	setSize(width, getHeight());
}

void ScenesBarComponent::paint(Graphics& g) {
	// höhö G-Punkt

	// draw ticks
	g.setColour(findColour(ScenesBarComponent::tickColourId));

	const int lineDistance             = 20;
	const int longLineDistance         = 5;
	const float lineHeightFraction     = 0.25;
	const float longLineHeightFraction = 0.5;

	for(int i = 0; i*lineDistance < getWidth(); i++){
		const bool longLine = (i%longLineDistance == 0);
		g.drawLine(
				i*lineDistance + 0.5,
				0,
				i*lineDistance + 0.5,
				getHeight()*(longLine ? longLineHeightFraction : lineHeightFraction),
				1
			);
	}

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
}

void ScenesBarComponent::paintOverChildren(Graphics& g) {
	// draw time marker
	g.setColour(findColour(ScenesBarComponent::timeMarkerColourId));
	const float x = data.currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);
}

void ScenesBarComponent::updateSceneComponents() {
	sceneComponentsArray.clearQuick(true);
	ValueTree sceneDatasArray = data.getScenesArray();
	const int numScenes = sceneDatasArray.getNumChildren();

	for (int i = 0; i < numScenes; i++) {
		ValueTree sceneData = sceneDatasArray.getChild(i);
		SceneComponent* sceneComponent = new SceneComponent(sceneData);
		addAndMakeVisible(sceneComponent);
		sceneComponentsArray.add(sceneComponent);
	}
}

void ScenesBarComponent::removeSceneComponent(const SceneComponent* toBeDeleted) {
	sceneComponentsArray.removeObject(toBeDeleted);
}

void ScenesBarComponent::mouseDown(const MouseEvent& event) {
	const int sceneId = data.getNewSceneId();
	newSceneData = ValueTree(treeId::scene);
	newSceneData.setProperty(treeId::sceneId, var(sceneId), nullptr);
	newSceneData.setProperty(treeId::sceneShaderSource, var("dummy" + String(sceneId) + ".glsl"), nullptr);
	newSceneData.setProperty(treeId::sceneStart, var(event.getMouseDownX()), nullptr);
	newSceneData.setProperty(treeId::sceneDuration, var(0), nullptr);

	newSceneComponent = new SceneComponent(newSceneData);
	addAndMakeVisible(newSceneComponent);
}

void ScenesBarComponent::mouseDrag(const MouseEvent& event) {
	const int gridWidth = 20;

	const int mouseDown = event.getMouseDownX();
	const int mouseDownGrid = roundFloatToInt(float(mouseDown) / float(gridWidth)) * gridWidth;

	const int mousePos = event.getPosition().getX();
	const int mousePosGrid = roundFloatToInt(float(mousePos) / float(gridWidth)) * gridWidth;

	const int distanceGrid = mousePosGrid - mouseDownGrid;
	const int absDistanceGrid = abs(distanceGrid);
	const int startGrid = mouseDownGrid + jmin(0, distanceGrid); // subtract distance if negative

	newSceneData.setProperty(treeId::sceneStart, var(startGrid), nullptr);
	newSceneData.setProperty(treeId::sceneDuration, var(absDistanceGrid), nullptr);
	newSceneComponent->updateBounds();
}

void ScenesBarComponent::mouseUp(const MouseEvent& event) {
	if (int(newSceneData.getProperty(treeId::sceneDuration)) == 0) {
		delete newSceneComponent;
	} else {
		sceneComponentsArray.add(newSceneComponent);
		data.addScene(newSceneData);
	}
	newSceneComponent = nullptr;
	newSceneData = ValueTree();
}

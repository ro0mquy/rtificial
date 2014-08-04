#include "ScenesBarComponent.h"
#include "TreeIdentifiers.h"
#include "../RtificialLookAndFeel.h"

ScenesBarComponent::ScenesBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data),
	newSceneData(treeId::scene)
{
	updateSceneComponents();
}

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

	// draw time marker
	g.setColour(findColour(ScenesBarComponent::timeMarkerColourId));
	const float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
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

void ScenesBarComponent::mouseDown(const MouseEvent& event) {
	newSceneData.setProperty(treeId::sceneId, var(23), nullptr);
	newSceneData.setProperty(treeId::sceneShaderSource, var("dummy.glsl"), nullptr);
	newSceneData.setProperty(treeId::sceneStart, var(event.getMouseDownX()), nullptr);
	newSceneData.setProperty(treeId::sceneDuration, var(0), nullptr);

	newSceneComponent = new SceneComponent(newSceneData);
	newSceneComponent->setVisible(false);
	addChildComponent(newSceneComponent);
}

void ScenesBarComponent::mouseDrag(const MouseEvent& event) {
	const int minDragDistance = 20;

	const int dragStart = event.getMouseDownX();
	const int dragDistance = event.getDistanceFromDragStartX();
	const int absoluteDragDistance = abs(dragDistance);
	const int start = dragStart + jmin(0, dragDistance); // subtract distance if negative

	newSceneComponent->setVisible(absoluteDragDistance >= minDragDistance);

	newSceneData.setProperty(treeId::sceneStart, var(start), nullptr);
	newSceneData.setProperty(treeId::sceneDuration, var(absoluteDragDistance), nullptr);
	newSceneComponent->updateBounds();
}

void ScenesBarComponent::mouseUp(const MouseEvent& event) {
	delete newSceneComponent;
	newSceneData.removeAllProperties(nullptr);
}

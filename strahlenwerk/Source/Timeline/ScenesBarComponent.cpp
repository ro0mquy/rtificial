#include "ScenesBarComponent.h"

#include "../RtificialLookAndFeel.h"
#include "TimelineData.h"
#include "SceneComponent.h"

ScenesBarComponent::ScenesBarComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
	zoomFactor.addListener(this);
	data.currentTime.addListener(this);
	updateSceneComponents();
}

ScenesBarComponent::~ScenesBarComponent() = default;

void ScenesBarComponent::updateSize() {
	const int paddingAfterLastScene = 300;
	const int endTime = data.getLastSceneEndTime() * zoomFactor;

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int width = jmax(endTime + paddingAfterLastScene, viewportWidth);
	setSize(width, viewportHeight);
}

void ScenesBarComponent::paint(Graphics& g) {
	// höhö G-Punkt

	// draw ticks
	g.setColour(findColour(ScenesBarComponent::tickColourId));

	const float lineDistance           = 20 * zoomFactor;
	const int longLineDistance         = 4;
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
	const float timeMarkerLineWidth = 2.;
	g.setColour(findColour(ScenesBarComponent::timeMarkerColourId));
	const float x = (float) data.currentTime.getValue() * zoomFactor;
	g.drawLine(x, 0, x, getHeight(), timeMarkerLineWidth);
}

void ScenesBarComponent::updateSceneComponents() {
	sceneComponentsArray.clearQuick(true);
	const int numScenes = data.getNumScenes();

	for (int i = 0; i < numScenes; i++) {
		ValueTree sceneData = data.getScene(i);
		SceneComponent* sceneComponent = new SceneComponent(sceneData, zoomFactor);
		addAndMakeVisible(sceneComponent);
		sceneComponentsArray.add(sceneComponent);
	}
}

void ScenesBarComponent::removeSceneComponent(const SceneComponent* toBeDeleted) {
	sceneComponentsArray.removeObject(toBeDeleted);
}

void ScenesBarComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown()) {
		var sceneStart = event.getMouseDownX() / zoomFactor;
		var sceneDuration = 0;
		var sceneShaderSource = "dummy" + String(data.getNewSceneId()) + ".glsl";
		newSceneData = data.addScene(sceneStart, sceneDuration, sceneShaderSource);

		newSceneComponent = new SceneComponent(newSceneData, zoomFactor);
		addAndMakeVisible(newSceneComponent);
	} else {
		McbComponent::mouseDown(event);
	}
}

void ScenesBarComponent::mouseDrag(const MouseEvent& event) {
	// invalid data happens on no left click or no command down
	if (newSceneData.isValid()) {
		const int gridWidth = 20; // time units

		const float mouseDown = event.getMouseDownX() / zoomFactor;
		const int mouseDownGrid = roundFloatToInt(mouseDown / float(gridWidth)) * gridWidth;

		const float mousePos = event.x / zoomFactor;
		const int mousePosGrid = roundFloatToInt(mousePos / float(gridWidth)) * gridWidth;

		const int distanceGrid = mousePosGrid - mouseDownGrid;
		const int absDistanceGrid = abs(distanceGrid);
		const int startGrid = mouseDownGrid + jmin(0, distanceGrid); // subtract distance if negative

		data.setSceneStart(newSceneData, startGrid);
		data.setSceneDuration(newSceneData, absDistanceGrid);
		newSceneComponent->updateBounds();
	} else {
		McbComponent::mouseDrag(event);
	}
}

void ScenesBarComponent::mouseUp(const MouseEvent& event) {
	// invalid data happens on no left click or no command down
	if (newSceneData.isValid()) {
		if (0 == int(data.getSceneDuration(newSceneData))) {
			newSceneComponent = nullptr;
			data.removeScene(newSceneData);
		} else {
			sceneComponentsArray.add(newSceneComponent.release());
		}
		newSceneData = ValueTree();
	} else {
		McbComponent::mouseUp(event);
	}
}

void ScenesBarComponent::zoomFactorChanged(ZoomFactor&) {
	updateSize();
	repaint();
}

void ScenesBarComponent::valueChanged(Value& /*value*/) {
	// currentTime changed
	repaint();
}

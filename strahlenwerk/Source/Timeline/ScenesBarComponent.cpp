#include "ScenesBarComponent.h"

#include <RtificialLookAndFeel.h>
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "SceneComponent.h"
#include "StrahlenwerkApplication.h"
#include "AudioManager.h"
#include "SnapToGridConstrainer.h"

ScenesBarComponent::ScenesBarComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
	data.addListenerToTree(this);
	data.currentTime.addListener(this);
	zoomFactor.addChangeListener(this);
	addAllSceneComponents();
}

ScenesBarComponent::~ScenesBarComponent() {
	data.removeListenerFromTree(this);
	data.currentTime.removeListener(this);
	zoomFactor.removeChangeListener(this);
}

void ScenesBarComponent::updateSize() {
	const float paddingAfterLastScene = 300;
	const float endTime = data.getLastSceneEndTime() * zoomFactor;

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int width = jmax(roundFloatToInt(endTime + paddingAfterLastScene), viewportWidth);
	setSize(width, viewportHeight);
}

void ScenesBarComponent::paint(Graphics& g) {
	// höhö G-Punkt

	auto& audioManager = StrahlenwerkApplication::getInstance()->getAudioManager();
	auto& audioThumb = audioManager.getThumbnail();
	const float beatsPerSecond = audioManager.getBpm() / 60.;
	const float timeAtRightBorder = getWidth() / zoomFactor * beatsPerSecond;
	audioThumb.drawChannel(g, getLocalBounds(), 0., timeAtRightBorder, 0, 1.);
	audioThumb.drawChannel(g, getLocalBounds(), 0., timeAtRightBorder, 1, 1.);

	// draw ticks
	g.setColour(findColour(ScenesBarComponent::tickColourId));

	const float lineDistance           = zoomFactor.getGridWith() * zoomFactor;
	const int longLineDistance         = 4; // every nth tick is a long line
	const float lineHeightFraction     = 0.25;
	const float longLineHeightFraction = 0.5;

	const int width = getWidth();
	const int height = getHeight();
	for(int i = 0; i*lineDistance < width; i++){
		const bool longLine = (i%longLineDistance == 0);
		g.drawLine(
				i*lineDistance + 0.5,
				0,
				i*lineDistance + 0.5,
				height * (longLine ? longLineHeightFraction : lineHeightFraction),
				1
			);
	}

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawHorizontalLine(height - 1, 0, width);
}

void ScenesBarComponent::paintOverChildren(Graphics& g) {
	// draw time marker
	const float timeMarkerLineWidth = 2.;
	g.setColour(findColour(ScenesBarComponent::timeMarkerColourId));
	const float x = (float) data.currentTime.getValue() * zoomFactor;
	g.drawLine(x, 0, x, getHeight(), timeMarkerLineWidth);
}

void ScenesBarComponent::addSceneComponent(ValueTree sceneData) {
	SceneComponent* sceneComponent = new SceneComponent(sceneData, zoomFactor);
	addAndMakeVisible(sceneComponent);
	sceneComponentsArray.add(sceneComponent);
}

void ScenesBarComponent::addAllSceneComponents() {
	sceneComponentsArray.clearQuick(true);
	const int numScenes = data.getNumScenes();

	for (int i = 0; i < numScenes; i++) {
		ValueTree sceneData = data.getScene(i);
		addSceneComponent(sceneData);
	}
}

SceneComponent* ScenesBarComponent::getSceneComponentForData(ValueTree sceneData) {
	const int componentsArraySize = sceneComponentsArray.size();
	for (int i = 0; i < componentsArraySize; i++) {
		auto sceneComponent = sceneComponentsArray.getUnchecked(i);
		if (sceneComponent->sceneData == sceneData) {
			return sceneComponent;
		}
	}
	return nullptr;
}

void ScenesBarComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown()) {
		var sceneStart = event.getMouseDownX() / zoomFactor;
		var sceneDuration = 0.f;
		var sceneShaderSource = "szenchen" + String(data.getNewSceneId());
		currentlyCreatedSceneData = data.addScene(sceneStart, sceneDuration, sceneShaderSource);
	} else {
		McbComponent::mouseDown(event);
	}
}

void ScenesBarComponent::mouseDrag(const MouseEvent& event) {
	// invalid data happens on no left click or no command down
	if (currentlyCreatedSceneData.isValid()) {
		const float mouseDown = event.getMouseDownX() / zoomFactor;
		const float mouseDownGrid = SnapToGridConstrainer::snapValueToGrid(mouseDown);

		const float mousePos = event.x / zoomFactor;
		const float mousePosGrid = SnapToGridConstrainer::snapValueToGrid(mousePos);

		const float distanceGrid = mousePosGrid - mouseDownGrid;
		const float absDistanceGrid = std::abs(distanceGrid);
		const float startGrid = mouseDownGrid + jmin(0.f, distanceGrid); // subtract distance if negative

		data.setSceneStart(currentlyCreatedSceneData, startGrid);
		data.setSceneDuration(currentlyCreatedSceneData, absDistanceGrid);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void ScenesBarComponent::mouseUp(const MouseEvent& event) {
	// invalid data happens on no left click or no command down
	if (currentlyCreatedSceneData.isValid()) {
		if (0. == float(data.getSceneDuration(currentlyCreatedSceneData))) {
			data.removeScene(currentlyCreatedSceneData);
		}
		currentlyCreatedSceneData = ValueTree();
	} else {
		McbComponent::mouseUp(event);
	}
}

void ScenesBarComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor update
	updateSize();
}

void ScenesBarComponent::valueChanged(Value& /*value*/) {
	// currentTime changed
	repaint();
}

// ValueTree::Listener callbacks
void ScenesBarComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& /*property*/) {
	if (parentTree.hasType(treeId::scene)) {
		updateSize();
	}
}

void ScenesBarComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (data.isScene(childWhichHasBeenAdded)) {
		addSceneComponent(childWhichHasBeenAdded);
	} else if (childWhichHasBeenAdded.hasType(treeId::scene)) {
		updateSize();
	}
}

void ScenesBarComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved) {
	if (data.isScene(childWhichHasBeenRemoved)) {
		auto sceneComponent = getSceneComponentForData(childWhichHasBeenRemoved);
		jassert(sceneComponent != nullptr);
		sceneComponentsArray.removeObject(sceneComponent);
	} else if (childWhichHasBeenRemoved.hasType(treeId::scene)) {
		updateSize();
	}
}

void ScenesBarComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void ScenesBarComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void ScenesBarComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always the root tree
	addAllSceneComponents();
	updateSize();
	repaint();
}

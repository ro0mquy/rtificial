#include "ScenesBarComponent.h"

#include <RtificialLookAndFeel.h>
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "SceneComponent.h"
#include <AudioManager.h>
#include "ZoomFactor.h"

ScenesBarComponent::ScenesBarComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	timeMarker(zoomFactor_)
{
	data.addListenerToTree(this);
	zoomFactor.addChangeListener(this);
	addAllSceneComponents();
	addAndMakeVisible(timeMarker);
	setBufferedToImage(true);
}

ScenesBarComponent::~ScenesBarComponent() {
	data.removeListenerFromTree(this);
	zoomFactor.removeChangeListener(this);
}

void ScenesBarComponent::updateSize() {
	const float paddingAfterLastScene = 300 + 18; // + getLookAndFeel().getDefaultScrollbarWidth(); // TODO: do this in a more dynamic way
	const float endTime = data.getLastSceneEndTime() * zoomFactor;

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int width = jmax(roundFloatToInt(endTime + paddingAfterLastScene), viewportWidth);
	setSize(width, viewportHeight);
}

void ScenesBarComponent::paint(Graphics& g) {
	// höhö G-Punkt

	// draw waveform thumb nail
	auto& audioManager = AudioManager::getAudioManager();
	auto& audioThumb = audioManager.getThumbnail();
	const float beatsPerSecond = audioManager.getBpm() / 60.;
	const float timeAtRightBorder = getWidth() / zoomFactor / beatsPerSecond;

	Rectangle<int> halfVisibleRect = getLocalBounds();
	halfVisibleRect.setHeight(2 * halfVisibleRect.getHeight());

	g.setColour(findColour(ScenesBarComponent::waveformColourId));
	audioThumb.drawChannel(g, halfVisibleRect, 0., timeAtRightBorder, 0, 1.);
	//audioThumb.drawChannel(g, halfVisibleRect, 0., timeAtRightBorder, 1, 1.);

	// draw ticks
	g.setColour(findColour(ScenesBarComponent::tickColourId));

	const float gridWidth              = zoomFactor.getGridWith();
	const float lineDistance           = gridWidth * zoomFactor;
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

		if (longLine) {
			g.setColour(findColour(ScenesBarComponent::textColourId));
			g.drawSingleLineText(String(i * gridWidth), i*lineDistance + 1, .8 * g.getCurrentFont().getHeight());
		}
	}

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawHorizontalLine(height - 1, 0, width);
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
		const float mouseDownGrid = zoomFactor.snapValueToGrid(mouseDown);

		const float mousePos = event.x / zoomFactor;
		const float mousePosGrid = zoomFactor.snapValueToGrid(mousePos);

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
	// zoomFactor changed
	updateSize();
	repaint();
}

// ValueTree::Listener callbacks
void ScenesBarComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& /*property*/) {
	if (parentTree.hasType(treeId::scene)) {
		updateSize();
		repaint();
	}
}

void ScenesBarComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (data.isScene(childWhichHasBeenAdded)) {
		addSceneComponent(childWhichHasBeenAdded);
	} else if (childWhichHasBeenAdded.hasType(treeId::scene)) {
		updateSize();
		repaint();
	}
}

void ScenesBarComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved) {
	if (data.isScene(childWhichHasBeenRemoved)) {
		auto sceneComponent = getSceneComponentForData(childWhichHasBeenRemoved);
		jassert(sceneComponent != nullptr);
		sceneComponentsArray.removeObject(sceneComponent);
	} else if (childWhichHasBeenRemoved.hasType(treeId::scene)) {
		updateSize();
		repaint();
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

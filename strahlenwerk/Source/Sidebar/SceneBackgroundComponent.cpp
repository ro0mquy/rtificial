#include "SceneBackgroundComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/ScenesBarComponent.h>
#include <AudioManager.h>
#include <RtificialLookAndFeel.h>

SceneBackgroundComponent::SceneBackgroundComponent(ValueTree sceneData_) :
	sceneData(sceneData_),
	timeMarker(sceneData),
	data(TimelineData::getTimelineData()),
	zoomFactor(ZoomFactor::getZoomFactor())
{
	data.addListenerToTree(this);
	zoomFactor.addChangeListener(this);

	resized();
	addAndMakeVisible(timeMarker);
}

SceneBackgroundComponent::~SceneBackgroundComponent() {
	data.removeListenerFromTree(this);
	zoomFactor.removeChangeListener(this);
}

void SceneBackgroundComponent::resized() {
	timeMarker.updatePosition();
}

void SceneBackgroundComponent::paint(Graphics& g) {
	const int sceneStart = data.getSceneStart(sceneData);
	const int sceneDuration = data.getSceneDuration(sceneData);
	const int sceneEnd = sceneStart + sceneDuration;

	const float scaleFactor = float(getWidth()) / float(sceneDuration);

	// draw ticks
	const int gridWidth = zoomFactor.getGridWidth();
	const int firstLine = sceneStart + ((gridWidth - (sceneStart % gridWidth)) % gridWidth); // round up if not already on the grid

	const int longLineDistance = 4; // every nth tick is a long line
	const float longLineHeight = 30. /* scenesBarHeight */ / 2.;
	const float lineHeight = longLineHeight / 2.;
	const float maxX = getWidth() - .5;

	for (int i = firstLine; i <= sceneEnd; i += gridWidth) {
		const bool longLine = ((i / gridWidth) % longLineDistance == 0);
		g.setColour(findColour(ScenesBarComponent::tickColourId));
		g.drawLine(
				jmin((i - sceneStart) * scaleFactor + 0.5f, maxX),
				0,
				jmin((i - sceneStart) * scaleFactor + 0.5f, maxX),
				longLine ? longLineHeight : lineHeight,
				1
			);

		if (longLine) {
			const String nthBeat(i / 1000.);
			float posX = (i - sceneStart) * scaleFactor + 1.f;
			const float baseLineY = .8 * g.getCurrentFont().getHeight();
			Justification justific = Justification::left;
			if (posX > maxX) {
				justific = Justification::right;
				posX -= 2.f;
			}
			g.setColour(findColour(ScenesBarComponent::textColourId));
			g.drawSingleLineText(nthBeat, posX, baseLineY, justific);
		}
	}

	// draw scene rect
	RtificialLookAndFeel* laf = dynamic_cast<RtificialLookAndFeel*>(&getLookAndFeel());

	const int rowHeight = 20;
	Rectangle<float> sceneRect = getLocalBounds().removeFromBottom(rowHeight).toFloat();

	const bool selected = true;

	if (nullptr == laf) {
		Colour fillColor = findColour(SceneComponent::fillColourId);
		if (selected) {
			fillColor = findColour(SceneComponent::highlightedFillColourId);
		}
		g.fillAll(fillColor);

		g.setColour(findColour(SceneComponent::outlineColourId));
		g.drawRect(sceneRect, 1);
	} else {
		laf->drawScene(g, sceneRect, selected, String::empty);
	}
}

void SceneBackgroundComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		// set current Time
		mouseDrag(event);
	} else {
		Component::mouseDown(event);
	}
}

void SceneBackgroundComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		if (CameraController::globalCameraController != nullptr) {
			if (m.isShiftDown()) {
				CameraController::globalCameraController->releaseControl();
			} else {
				CameraController::globalCameraController->takeOverControl();
			}
		}

		const int sceneStart = data.getSceneStart(sceneData);
		const float sceneDuration = data.getSceneDuration(sceneData);

		const int relativeNewTime = (float) event.x / getWidth() * sceneDuration;
		const int absoluteNewTime = jmax(sceneStart + relativeNewTime, 0);
		const int absoluteNewTimeGrid = zoomFactor.snapValueToGrid(absoluteNewTime);
		AudioManager::getAudioManager().setTime(absoluteNewTimeGrid);
	} else {
		Component::mouseDrag(event);
	}
}

void SceneBackgroundComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor changed
	repaint();
}

// ValueTree::Listener callbacks
void SceneBackgroundComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& /*property*/) {
	if (parentTree == sceneData) {
		// any of the scene properties changed
		repaint();
		timeMarker.updatePosition();
	}
}

void SceneBackgroundComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
}

void SceneBackgroundComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
}

void SceneBackgroundComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void SceneBackgroundComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

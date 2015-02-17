#include "SequenceBackgroundComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/ScenesBarComponent.h>
#include <AudioManager.h>

SequenceBackgroundComponent::SequenceBackgroundComponent(ValueTree sequenceData_) :
	sequenceData(sequenceData_),
	sequenceComponent(sequenceData),
	timeMarker(sequenceData),
	data(TimelineData::getTimelineData()),
	zoomFactor(ZoomFactor::getZoomFactor())
{
	data.addListenerToTree(this);
	zoomFactor.addChangeListener(this);

	resized();
	addAndMakeVisible(sequenceComponent);
	addAndMakeVisible(timeMarker);
}

SequenceBackgroundComponent::~SequenceBackgroundComponent() {
	data.removeListenerFromTree(this);
	zoomFactor.removeChangeListener(this);
}

void SequenceBackgroundComponent::resized() {
	const int rowHeight = 20;
	sequenceComponent.setBounds(getLocalBounds().removeFromBottom(rowHeight));
	timeMarker.updatePosition();
}

void SequenceBackgroundComponent::paint(Graphics& g) {
	const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
	const int sequenceDuration = data.getSequenceDuration(sequenceData);
	const int sequenceEnd = sequenceStart + sequenceDuration;

	const float scaleFactor = float(getWidth()) / float(sequenceDuration);

	// draw ticks
	const int gridWidth = zoomFactor.getGridWidth();
	const int firstLine = sequenceStart + ((gridWidth - (sequenceStart % gridWidth)) % gridWidth); // round up if not already on the grid

	const int longLineDistance = 4; // every nth tick is a long line
	const float longLineHeight = 30. /* scenesBarHeight */ / 2.;
	const float lineHeight = longLineHeight / 2.;
	const float maxX = getWidth() - .5;

	for (int i = firstLine; i <= sequenceEnd; i += gridWidth) {
		const bool longLine = ((i / gridWidth) % longLineDistance == 0);
		g.setColour(findColour(ScenesBarComponent::tickColourId));
		g.drawLine(
				jmin((i - sequenceStart) * scaleFactor + 0.5f, maxX),
				0,
				jmin((i - sequenceStart) * scaleFactor + 0.5f, maxX),
				longLine ? longLineHeight : lineHeight,
				1
			);

		if (longLine) {
			const String nthBeat(i / 1000.);
			float posX = (i - sequenceStart) * scaleFactor + 1.f;
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
}

void SequenceBackgroundComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		// set current Time
		mouseDrag(event);
	} else {
		Component::mouseDown(event);
	}
}

void SequenceBackgroundComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		if (CameraController::globalCameraController != nullptr) {
			if (m.isShiftDown()) {
				CameraController::globalCameraController->releaseControl();
			} else {
				CameraController::globalCameraController->takeOverControl();
			}
		}

		const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
		const float sequenceDuration = data.getSequenceDuration(sequenceData);

		const int relativeNewTime = (float) event.x / getWidth() * sequenceDuration;
		const int absoluteNewTime = jmax(sequenceStart + relativeNewTime, 0);
		const int absoluteNewTimeGrid = zoomFactor.snapValueToGrid(absoluteNewTime);
		AudioManager::getAudioManager().setTime(absoluteNewTimeGrid);
	} else {
		Component::mouseDrag(event);
	}
}

void SequenceBackgroundComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor changed
	repaint();
}

// ValueTree::Listener callbacks
void SequenceBackgroundComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree == sequenceData) {
		// any of the sequence properties changed
		repaint();
		timeMarker.updatePosition();
	} else if (property == treeId::sceneStart) {
		if (parentTree == data.getScene(data.getSequenceSceneId(sequenceData))) {
			// the scene this sequence belongs to has been moved
			repaint();
			timeMarker.updatePosition();
		}
	}
}

void SequenceBackgroundComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
}

void SequenceBackgroundComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/) {
}

void SequenceBackgroundComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void SequenceBackgroundComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

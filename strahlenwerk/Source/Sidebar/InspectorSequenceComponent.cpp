#include "InspectorSequenceComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/ScenesBarComponent.h>
#include <Timeline/SequenceComponent.h>
#include <AudioManager.h>

InspectorSequenceComponent::InspectorSequenceComponent(ValueTree sequenceData_) :
	sequenceData(sequenceData_),
	data(TimelineData::getTimelineData()),
	audioManager(AudioManager::getAudioManager()),
	zoomFactor(ZoomFactor::getZoomFactor()),
	timeMarker(sequenceData)
{
	data.addListenerToTree(this);
	addAllKeyframeComponents();
	addAndMakeVisible(timeMarker);
}

InspectorSequenceComponent::~InspectorSequenceComponent() {
	data.removeListenerFromTree(this);
}

void InspectorSequenceComponent::paint(Graphics& g) {
	const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
	const int sequenceDuration = data.getSequenceDuration(sequenceData);
	const int sequenceEnd = sequenceStart + sequenceDuration;

	const float scaleFactor = float(getWidth()) / float(sequenceDuration);

	// draw ticks
	const int gridWidth = zoomFactor.getGridWidth();
	const int firstLine = sequenceStart + ((gridWidth - (sequenceStart % gridWidth)) % gridWidth); // round up if not already on the grid

	const int longLineDistance = 4; // every nth tick is a long line
	const float longLineHeight = 30. / 2.;
	const float lineHeight = longLineHeight / 2.;

	for (int i = firstLine; i <= sequenceEnd; i += gridWidth) {
		const bool longLine = ((i / gridWidth) % longLineDistance == 0);
		g.setColour(findColour(ScenesBarComponent::tickColourId));
		g.drawLine(
				(i - sequenceStart) * scaleFactor + 0.5,
				0,
				(i - sequenceStart) * scaleFactor + 0.5,
				longLine ? longLineHeight : lineHeight,
				1
			);

		if (longLine) {
			g.setColour(findColour(ScenesBarComponent::textColourId));
			g.drawSingleLineText(String(i / 1000.), (i - sequenceStart) * scaleFactor + 1, .8 * g.getCurrentFont().getHeight());
		}
	}

	// draw sequence
	const float cornerSize = 5.0;
	Rectangle<float> boundsSequence = getLocalBounds().toFloat().withTrimmedTop(longLineHeight);

	g.setColour(findColour(SequenceComponent::fillColourId));
	g.fillRoundedRectangle(boundsSequence, cornerSize);

	g.setColour(findColour(SequenceComponent::outlineColourId));
	g.drawRoundedRectangle(boundsSequence, cornerSize, 1);
}

void InspectorSequenceComponent::addKeyframeComponent(ValueTree keyframeData) {
	auto keyframeComponent = new InspectorKeyframeComponent(*this, keyframeData, zoomFactor);
	addAndMakeVisible(keyframeComponent);
	keyframeComponentsArray.add(keyframeComponent);
}

void InspectorSequenceComponent::addAllKeyframeComponents() {
	keyframeComponentsArray.clearQuick(true);
	const int numKeyframes = data.getNumKeyframes(sequenceData);

	// don't create a component for first and last keyframe
	for (int i = 1; i < numKeyframes - 1; i++) {
		ValueTree keyframeData = data.getKeyframe(sequenceData, i);
		addKeyframeComponent(keyframeData);
	}
}

KeyframeComponent* InspectorSequenceComponent::getKeyframeComponentForData(ValueTree keyframeData) {
	const int componentsArraySize = keyframeComponentsArray.size();
	for (int i = 0; i < componentsArraySize; i++) {
		auto keyframeComponent = keyframeComponentsArray.getUnchecked(i);
		if (keyframeComponent->keyframeData == keyframeData) {
			return keyframeComponent;
		}
	}
	return nullptr;
}

void InspectorSequenceComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.mouseWasClicked() && m.isCommandDown() && m.isLeftButtonDown()) {
		// add keyframe
		const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
		const int sequenceDuration = data.getSequenceDuration(sequenceData);

		const int relativeMouseDown = (float) event.getMouseDownX() / getWidth() * sequenceDuration;
		const int absoluteMouseDown = relativeMouseDown + sequenceStart;

		const int absoluteMouseDownGrid = zoomFactor.snapValueToGrid(absoluteMouseDown);
		const int relativeMouseDownGrid = absoluteMouseDownGrid - sequenceStart;

		if (relativeMouseDownGrid > 0 && relativeMouseDownGrid < sequenceDuration) {
			// don't set keyframe at start or end
			data.getUndoManager().beginNewTransaction("Create Keyframe");
			data.addKeyframe(sequenceData, relativeMouseDownGrid);
		}

	} else {
		Component::mouseUp(event);
	}
}

// ValueTree::Listener callbacks
void InspectorSequenceComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree == sequenceData) {
		// any of the sequence properties changed
		repaint();
	} else if (property == treeId::sceneStart) {
		if (parentTree == data.getScene(data.getSequenceSceneId(sequenceData))) {
			// the scene this sequence belongs to has been moved
			repaint();
		}
	}
}

void InspectorSequenceComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
	// TODO: react on addition of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		const int keyframeIndex = data.getKeyframeIndex(childWhichHasBeenAdded);
		const int numKeyframes = data.getNumKeyframes(sequenceData);
		if (keyframeIndex == 0 || keyframeIndex == numKeyframes - 1) {
			// don't add a component for first or last keyframe
			return;
		}
		addKeyframeComponent(childWhichHasBeenAdded);
	}
}

void InspectorSequenceComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
	// TODO: react on removal of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		auto keyframeComponent = getKeyframeComponentForData(childWhichHasBeenRemoved);
		// keyframe could be start or end keyframe
		if (keyframeComponent != nullptr) {
			keyframeComponentsArray.removeObject(keyframeComponent);
		}
	}
}

void InspectorSequenceComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void InspectorSequenceComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

#include "InspectorSequenceComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/SequenceComponent.h>

InspectorSequenceComponent::InspectorSequenceComponent(ValueTree sequenceData_) :
	sequenceData(sequenceData_),
	data(TimelineData::getTimelineData()),
	zoomFactor(ZoomFactor::getZoomFactor())
{
	data.addListenerToTree(this);
	addAllKeyframeComponents();
}

InspectorSequenceComponent::~InspectorSequenceComponent() {
	data.removeListenerFromTree(this);
}

void InspectorSequenceComponent::resized() {
	const int numKeyframes = keyframeComponentsArray.size();
	for (int i = 0; i < numKeyframes; i++) {
		keyframeComponentsArray.getUnchecked(i)->updateBounds();
	}
}

void InspectorSequenceComponent::paint(Graphics& g) {
	// draw sequence
	const float cornerSize = 5.0;
	Rectangle<float> boundsSequence = getLocalBounds().toFloat();

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

	for (int i = 0; i < numKeyframes; i++) {
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

		if (relativeMouseDownGrid >= 0 && relativeMouseDownGrid <= sequenceDuration) {
			// don't set keyframe beyond start or end
			data.getUndoManager().beginNewTransaction("Create Keyframe");
			data.addKeyframe(sequenceData, relativeMouseDownGrid);
		}

	} else if (event.mouseWasClicked() && m.isRightButtonDown() && !m.isAnyModifierKeyDown()) {
		// add sequence to selection
		data.getSelection().set(sequenceData);

	} else {
		Component::mouseUp(event);
	}
}

// ValueTree::Listener callbacks
void InspectorSequenceComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
}

void InspectorSequenceComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
	// TODO: react on addition of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		addKeyframeComponent(childWhichHasBeenAdded);
	}
}

void InspectorSequenceComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int /*indexFromWhichChildWasRemoved*/) {
	// TODO: react on removal of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		KeyframeComponent* keyframeComponent = getKeyframeComponentForData(childWhichHasBeenRemoved);
		keyframeComponentsArray.removeObject(keyframeComponent);
	}
}

void InspectorSequenceComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void InspectorSequenceComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

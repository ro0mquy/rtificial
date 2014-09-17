#include "SequenceComponent.h"

#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "KeyframeComponent.h"
#include "SequenceViewComponent.h"

SequenceComponent::SequenceComponent(ValueTree _sequenceData, ZoomFactor& zoomFactor_, int y, int height) :
	sequenceData(_sequenceData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	// register for changes of the whole timeline tree
	data.addListenerToTree(this);

	// register for zoom factor changes
	zoomFactor.addListener(this);

	// set the initial y-coordinate and height
	updateBounds();
	setBounds(getBounds().withY(y).withHeight(height));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(20);
	constrainer.setMinimumWidth(20);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	// add keyframe components
	addAllKeyframeComponents();
}

void SequenceComponent::updateBounds() {
	const float start = (float) data.getAbsoluteStartForSequence(sequenceData) * zoomFactor;
	const float duration = (float) data.getSequenceDuration(sequenceData) * zoomFactor;

	Rectangle<int> bounds = getBounds();
	bounds.setX(start);
	bounds.setWidth(duration);
	setBounds(bounds);
}

void SequenceComponent::addKeyframeComponent(ValueTree keyframeData) {
	auto keyframeComponent = new KeyframeComponent(keyframeData, zoomFactor);
	addAndMakeVisible(keyframeComponent);
	keyframeComponentsArray.add(keyframeComponent);
}

void SequenceComponent::addAllKeyframeComponents() {
	keyframeComponentsArray.clearQuick(true);
	const int numKeyframes = data.getNumKeyframes(sequenceData);

	// don't create a component for first and last keyframe
	for (int i = 1; i < numKeyframes - 1; i++) {
		ValueTree keyframeData = data.getKeyframe(sequenceData, i);
		addKeyframeComponent(keyframeData);
	}
}

KeyframeComponent* SequenceComponent::getKeyframeComponentForData(ValueTree keyframeData) {
	const int componentsArraySize = keyframeComponentsArray.size();
	for (int i = 0; i < componentsArraySize; i++) {
		auto keyframeComponent = keyframeComponentsArray.getUnchecked(i);
		if (keyframeComponent->keyframeData == keyframeData) {
			return keyframeComponent;
		}
	}
	return nullptr;
}

void SequenceComponent::paint(Graphics& g) {
	const float cornerSize = 5.0;
	Rectangle<float> seqRect = getLocalBounds().toFloat();
	seqRect.removeFromTop(0.5);
	seqRect.removeFromBottom(1.5);

	g.setColour(findColour(SequenceComponent::fillColourId));
	g.fillRoundedRectangle(seqRect, cornerSize);

	g.setColour(findColour(SequenceComponent::outlineColourId));
	g.drawRoundedRectangle(seqRect, cornerSize, 1);
}

void SequenceComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown()) {
		beginDragAutoRepeat(10); // time between drag events
		startDraggingComponent(this, event);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SequenceComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (!event.mouseWasClicked() && m.isLeftButtonDown() && m.isCommandDown()) {
		dragComponent(this, event, &constrainer);

		// scroll viewport if necessary
		Viewport* parentViewport = findParentComponentOfClass<Viewport>();
		const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
		// scroll only X- not Y-Direction
		// current X position gets normally set
		// current Y position is a constant that is greater than the minimum distance to the border (21 > 20)
		parentViewport->autoScroll(viewportEvent.x, 21, 20, 5);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void SequenceComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.mouseWasClicked() && m.isCommandDown() && (m.isLeftButtonDown() || m.isMiddleButtonDown())) {
		if (m.isLeftButtonDown()) {
			// add keyframe
			const int gridWidth = 20;

			const float mouseDown = event.getMouseDownX() * zoomFactor;
			const int mouseDownGrid = roundFloatToInt(mouseDown / float(gridWidth)) * gridWidth;

			const int sequenceDuration = data.getSequenceDuration(sequenceData);
			if (mouseDownGrid == 0 || mouseDownGrid == sequenceDuration) {
				// don't set keyframe at start or end
				return;
			}

			ValueTree keyframeData = data.addKeyframe(sequenceData, mouseDownGrid);

			auto keyframeComponent = new KeyframeComponent(keyframeData, zoomFactor);
			addAndMakeVisible(keyframeComponent);
			keyframeComponentsArray.add(keyframeComponent);

		} else if (m.isMiddleButtonDown()) {
			// delete sequence
			AlertWindow reallyDeleteWindow("Sequence", L"Delëte this Sequence for a Long Time?", AlertWindow::WarningIcon);
			reallyDeleteWindow.addButton("Cancel", 0, KeyPress('c'), KeyPress(KeyPress::escapeKey));
			reallyDeleteWindow.addButton("Delete", 1, KeyPress('d'), KeyPress(KeyPress::spaceKey));

			const int returnedChoice = reallyDeleteWindow.runModalLoop();
			if (returnedChoice != 1) {
				return;
			}

			data.removeSequence(sequenceData);
			findParentComponentOfClass<SequenceViewComponent>()->removeSequenceComponent(this);
		}
	} else {
		McbComponent::mouseUp(event);
	}
}

void SequenceComponent::moved() {
	// update the sceneId and relativ start time
	const int newStart = constrainer.snapValueToGrid(getX() / zoomFactor);
	data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
}

void SequenceComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());

	const int newDuration = constrainer.snapValueToGrid(getWidth() / zoomFactor);
	data.setSequenceDuration(sequenceData, newDuration);
}

void SequenceComponent::zoomFactorChanged(ZoomFactor&) {
	updateBounds();
}

// ValueTree::Listener callbacks
void SequenceComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree == sequenceData) {
		// any of the sequence properties changed
		// (may ignore sequenceInterpolation change?)
		updateBounds();
	} else if (property == treeId::sceneStart) {
		if (parentTree == data.getScene(data.getSequenceSceneId(sequenceData))) {
			// the scene this sequence belongs to has been moved
			updateBounds();
		} else {
			// another scene has been moved, so maybe this sequence now belongs to another scene
			const int absoluteStart = data.getAbsoluteStartForSequence(sequenceData);
			data.setSequencePropertiesForAbsoluteStart(sequenceData, absoluteStart);
		}
	} else if (property == treeId::sceneDuration) {
			const int absoluteStart = data.getAbsoluteStartForSequence(sequenceData);
			data.setSequencePropertiesForAbsoluteStart(sequenceData, absoluteStart);
	}
}

void SequenceComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		addKeyframeComponent(childWhichHasBeenAdded);
	}
}

void SequenceComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		auto keyframeComponent = getKeyframeComponentForData(childWhichHasBeenRemoved);
		keyframeComponentsArray.removeObject(keyframeComponent);
	}
}

void SequenceComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void SequenceComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

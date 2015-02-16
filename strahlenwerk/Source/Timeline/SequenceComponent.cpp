#include "SequenceComponent.h"

#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "ZoomFactor.h"
#include "KeyframeComponent.h"
#include <RtificialLookAndFeel.h>

SequenceComponent::SequenceComponent(ValueTree _sequenceData, ZoomFactor& zoomFactor_) :
	sequenceData(_sequenceData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	// register for changes of the whole timeline tree and selections
	data.addListenerToTree(this);
	data.getSelection().addChangeListener(this);

	// register for zoom factor changes
	zoomFactor.addChangeListener(this);

	// set size and position
	updateBounds();

	// positioner that stores the results of drags into the sequence data
	setPositioner(new Positioner(*this, sequenceData, data, zoomFactor));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setMinimumWidth(zoomFactor.getGridWidth() * zoomFactor);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	// add keyframe components
	addAllKeyframeComponents();
}

SequenceComponent::Positioner::Positioner(Component& component, ValueTree sequenceData_, TimelineData& data_, ZoomFactor& zoomFactor_) :
	Component::Positioner(component),
	sequenceData(sequenceData_),
	data(data_),
	zoomFactor(zoomFactor_)
{
}

SequenceComponent::~SequenceComponent() {
	data.removeListenerFromTree(this);
	data.getSelection().removeChangeListener(this);
	zoomFactor.removeChangeListener(this);
}

void SequenceComponent::updateBounds() {
	const int rowHeight = 20;
	const int start = (int) data.getAbsoluteStartForSequence(sequenceData) * zoomFactor;
	const int newX = roundFloatToInt(start);
	const int newWidth = (int) data.getSequenceDuration(sequenceData) * zoomFactor;
	const int nthUniform = data.getUniformIndex(data.getSequenceParentUniform(sequenceData));
	jassert(nthUniform >= 0);

	setBounds(newX, nthUniform * rowHeight, newWidth, rowHeight);
}

void SequenceComponent::Positioner::applyNewBounds(const Rectangle<int>& newBounds) {
	const bool xChanged = newBounds.getX() != getComponent().getX();
	const bool widthChanged = newBounds.getWidth() != getComponent().getWidth();

	if (xChanged && !widthChanged) {
		// dragging
		const int newX = newBounds.getX() / zoomFactor;
		const int newStart = zoomFactor.snapValueToGrid(newX);
		data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
	} else if (xChanged && widthChanged) {
		// stretching left
		const int newX = newBounds.getX() / zoomFactor;
		const int newStart = zoomFactor.snapValueToGrid(newX);

		// calculate the new width through the change of X
		// so that the right end stays where it is
		// otherwise the duration and not the right side would be snapped to grid
		const int oldStart = data.getAbsoluteStartForSequence(sequenceData);
		const int deltaStart = newStart - oldStart;
		const int oldDuration = data.getSequenceDuration(sequenceData);
		const int newDuration = oldDuration - deltaStart;

		data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
		data.setSequenceDuration(sequenceData, newDuration);
	} else if (!xChanged && widthChanged) {
		// stretching right
		const int newWidth = newBounds.getWidth() / zoomFactor;
		const int newDuration = zoomFactor.snapValueToGrid(newWidth);
		data.setSequenceDuration(sequenceData, newDuration);
	}
}

void SequenceComponent::addKeyframeComponent(ValueTree keyframeData) {
	auto keyframeComponent = new TimelineKeyframeComponent(keyframeData, zoomFactor);
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
	RtificialLookAndFeel* laf = dynamic_cast<RtificialLookAndFeel*>(&getLookAndFeel());

	Rectangle<float> seqRect = getLocalBounds().toFloat();
	seqRect.removeFromTop(0.5f);
	seqRect.removeFromBottom(1.5f);

	const bool selected = data.getSelection().contains(sequenceData);

	if (nullptr == laf) {
		Colour fillColor = findColour(SequenceComponent::fillColourId);
		if (selected) {
			fillColor = findColour(SequenceComponent::highlightedFillColourId);
		}
		g.fillAll(fillColor);

		g.setColour(findColour(SequenceComponent::outlineColourId));
		g.drawRect(seqRect, 1);

		g.setColour(findColour(SequenceComponent::textColourId));
		g.drawText(getInterpolationMethod(), seqRect, Justification::centred, true);
	} else {
		laf->drawSequence(g, *this, seqRect, selected);
	}
}

void SequenceComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown()) {
		data.getUndoManager().beginNewTransaction("Drag Sequence");
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
	if (event.mouseWasClicked() && m.isCommandDown() && m.isLeftButtonDown()) {
		// add keyframe
		const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
		const int sequenceDuration = data.getSequenceDuration(sequenceData);

		const int relativeMouseDown = event.getMouseDownX() / zoomFactor;
		const int absoluteMouseDown = relativeMouseDown + sequenceStart;

		const int absoluteMouseDownGrid = zoomFactor.snapValueToGrid(absoluteMouseDown);
		const int relativeMouseDownGrid = absoluteMouseDownGrid - sequenceStart;

		if (relativeMouseDownGrid > 0 && relativeMouseDownGrid < sequenceDuration) {
			// don't set keyframe at start or end
			data.getUndoManager().beginNewTransaction("Create Keyframe");
			data.addKeyframe(sequenceData, relativeMouseDownGrid);
		}

	} else if (event.mouseWasClicked() && m.isCommandDown() && m.isPopupMenu()) {
		// select interpolation method
		String interpolationMethods[] = { "step", "linear", "ccrSpline" };
		const int numMethods = numElementsInArray(interpolationMethods);
		const String currentMethod = data.getSequenceInterpolation(sequenceData);
		int currentMethodId = 0;

		PopupMenu menu;
		for (int i = 0; i < numMethods; i++) {
			const String& itemText = interpolationMethods[i];
			const bool isTicked = itemText == currentMethod;
			if (isTicked) { currentMethodId = i+1; }
			menu.addItem(i+1, itemText, true, isTicked);
		}

		const int menuResult = menu.showAt(this, currentMethodId, getWidth(), 1, getHeight());
		if (menuResult == 0) {
			// user dismissed menu
			return;
		}

		// set method to selected one
		data.getUndoManager().beginNewTransaction("Change Interpolation");
		data.setSequenceInterpolation(sequenceData, interpolationMethods[menuResult - 1]);

	} else if (event.mouseWasClicked() && m.isCommandDown() && m.isMiddleButtonDown()) {
		// delete sequence
		AlertWindow reallyDeleteWindow("Sequence", L"Delëte this Sequence for a Long Time?", AlertWindow::WarningIcon);
		reallyDeleteWindow.addButton("Cancel", 0, KeyPress('c'), KeyPress(KeyPress::escapeKey));
		reallyDeleteWindow.addButton("Delete", 1, KeyPress('d'), KeyPress(KeyPress::spaceKey));

		const int returnedChoice = reallyDeleteWindow.runModalLoop();
		if (returnedChoice != 1) {
			return;
		}

		data.getUndoManager().beginNewTransaction("Remove Sequence");
		data.removeSequence(sequenceData);
		// this component gets deleted after this, so don't do stupid things
	} else if (event.mouseWasClicked() && m.isRightButtonDown() && !m.isAnyModifierKeyDown()) {
		// add sequence to selection
		data.getSelection().set(sequenceData);
	} else {
		McbComponent::mouseUp(event);
	}
}

void SequenceComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());
}

void SequenceComponent::changeListenerCallback(ChangeBroadcaster* source) {
	// zoomFactor update
	if (source == &zoomFactor) {
		constrainer.setMinimumWidth(zoomFactor.getGridWidth() * zoomFactor);
		updateBounds();
	} else if (source == &data.getSelection()) {
		repaint();
	}
}

// ValueTree::Listener callbacks
void SequenceComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree == sequenceData) {
		// any of the sequence properties changed
		updateBounds();
		repaint();
	} else if (property == treeId::sceneStart) {
		if (parentTree == data.getScene(data.getSequenceSceneId(sequenceData))) {
			// the scene this sequence belongs to has been moved
			updateBounds();
		}
	}
}

void SequenceComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
	// TODO: react on addition of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		const int keyframeIndex = data.getKeyframeIndex(childWhichHasBeenAdded);
		const int numKeyframes = data.getNumKeyframes(sequenceData);
		if (keyframeIndex == 0 || keyframeIndex == numKeyframes - 1) {
			// don't add a component for first or last keyframe
			return;
		}
		addKeyframeComponent(childWhichHasBeenAdded);
	} else if (childWhichHasBeenAdded.hasType(treeId::uniform)) {
		// uniform got inserted, row number may changed
		updateBounds();
	}
}

void SequenceComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
	// TODO: react on removal of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		auto keyframeComponent = getKeyframeComponentForData(childWhichHasBeenRemoved);
		// keyframe could be start or end keyframe
		if (keyframeComponent != nullptr) {
			keyframeComponentsArray.removeObject(keyframeComponent);
		}
	} else if (childWhichHasBeenRemoved.hasType(treeId::uniform)) {
		// uniform got inserted, row number may changed
		updateBounds();
	}
}

void SequenceComponent::valueTreeChildOrderChanged(ValueTree& parentTree) {
	if (parentTree == data.getUniformsArray()) {
		// uniforms order changed, row number may changed
		updateBounds();
	}
}

void SequenceComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

const String SequenceComponent::getInterpolationMethod() {
	return data.getSequenceInterpolation(sequenceData);
}

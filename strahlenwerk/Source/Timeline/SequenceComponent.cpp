#include "SequenceComponent.h"

#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "ZoomFactor.h"
#include "KeyframeComponent.h"

SequenceComponent::SequenceComponent(ValueTree _sequenceData, ZoomFactor& zoomFactor_) :
	sequenceData(_sequenceData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	// register for changes of the whole timeline tree
	data.addListenerToTree(this);

	// register for zoom factor changes
	zoomFactor.addChangeListener(this);

	// set size and position
	updateBounds();

	// positioner that stores the results of drags into the sequence data
	setPositioner(new Positioner(*this, sequenceData, data, zoomFactor));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setMinimumWidth(zoomFactor.getGridWith() * zoomFactor);

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
	zoomFactor.removeChangeListener(this);
}

void SequenceComponent::updateBounds() {
	const int rowHeight = 20;
	const float start = (float) data.getAbsoluteStartForSequence(sequenceData) * zoomFactor;
	const int newX = roundFloatToInt(start);
	const float duration = (float) data.getSequenceDuration(sequenceData) * zoomFactor;
	const int newWidth = roundFloatToInt(duration);
	const int nthUniform = data.getUniformIndex(data.getSequenceParentUniform(sequenceData));
	jassert(nthUniform >= 0);

	setBounds(newX, nthUniform * rowHeight, newWidth, rowHeight);
}

void SequenceComponent::Positioner::applyNewBounds(const Rectangle<int>& newBounds) {
	const bool xChanged = newBounds.getX() != getComponent().getX();
	const bool widthChanged = newBounds.getWidth() != getComponent().getWidth();

	if (xChanged && !widthChanged) {
		// dragging
		const float newX = newBounds.getX() / zoomFactor;
		const float newStart = zoomFactor.snapValueToGrid(newX);
		data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
	} else if (xChanged && widthChanged) {
		// stretching left
		const float newX = newBounds.getX() / zoomFactor;
		const float newStart = zoomFactor.snapValueToGrid(newX);

		// calculate the new width through the change of X
		// so that the right end stays where it is
		// otherwise the duration and not the right side would be snapped to grid
		const float oldStart = data.getAbsoluteStartForSequence(sequenceData);
		const float deltaStart = newStart - oldStart;
		const float oldDuration = data.getSequenceDuration(sequenceData);
		const float newDuration = oldDuration - deltaStart;

		data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
		data.setSequenceDuration(sequenceData, newDuration);
	} else if (!xChanged && widthChanged) {
		// stretching right
		const float newWidth = newBounds.getWidth() / zoomFactor;
		const float newDuration = zoomFactor.snapValueToGrid(newWidth);
		data.setSequenceDuration(sequenceData, newDuration);
	}
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

	const String interpolationMethod = data.getSequenceInterpolation(sequenceData);
	g.setColour(findColour(SequenceComponent::textColourId));
	g.drawText(interpolationMethod, seqRect, Justification::centred, true);
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
	if (event.mouseWasClicked() && m.isCommandDown() && (m.isLeftButtonDown() || m.isMiddleButtonDown() || m.isPopupMenu())) {
		if (m.isLeftButtonDown()) {
			// add keyframe
			// TODO: compute with absolute time values
			const float mouseDown = event.getMouseDownX() / zoomFactor;
			const float mouseDownGrid = zoomFactor.snapValueToGrid(mouseDown);

			const float sequenceDuration = data.getSequenceDuration(sequenceData);
			if (mouseDownGrid <= 0 || mouseDownGrid >= sequenceDuration) {
				// don't set keyframe at start or end
				return;
			}

			data.addKeyframe(sequenceData, mouseDownGrid);

		} else if (m.isPopupMenu()) {
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
			data.setSequenceInterpolation(sequenceData, interpolationMethods[menuResult - 1]);

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
			// this component gets deleted after this, so don't do stupid things
		}
	} else {
		McbComponent::mouseUp(event);
	}
}

void SequenceComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());
}

void SequenceComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor update
	constrainer.setMinimumWidth(zoomFactor.getGridWith() * zoomFactor);
	updateBounds();
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
		} else {
			if (data.getSequenceSceneId(sequenceData) == var::null) {
				// a scene has been moved, so maybe this sequence now belongs to another scene
				const float absoluteStart = data.getAbsoluteStartForSequence(sequenceData);
				data.setSequencePropertiesForAbsoluteStart(sequenceData, absoluteStart);
			}
		}
	} else if (property == treeId::sceneDuration) {
		if (data.getSequenceSceneId(sequenceData) == var::null) {
			const float absoluteStart = data.getAbsoluteStartForSequence(sequenceData);
			data.setSequencePropertiesForAbsoluteStart(sequenceData, absoluteStart);
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
	}
}

void SequenceComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
	// TODO: react on removal of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		auto keyframeComponent = getKeyframeComponentForData(childWhichHasBeenRemoved);
		jassert(keyframeComponent != nullptr);
		keyframeComponentsArray.removeObject(keyframeComponent);
	}
}

void SequenceComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void SequenceComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

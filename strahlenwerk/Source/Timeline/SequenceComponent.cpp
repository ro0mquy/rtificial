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
	const int start = (int) data.getAbsoluteStartForSequence(sequenceData) * zoomFactor;
	const int newX = roundFloatToInt(start);
	const int newWidth = (int) data.getSequenceDuration(sequenceData) * zoomFactor;

	setBounds(newX, getY(), newWidth, getHeight());
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
		int newStart = zoomFactor.snapValueToGrid(newX);

		const int oldStart = data.getAbsoluteStartForSequence(sequenceData);

		ValueTree firstKeyframe = data.getKeyframe(sequenceData, 0);
		if (firstKeyframe.isValid()) {
			const int posFirstKeyframe = data.getKeyframePosition(firstKeyframe);
			newStart = jmin(newStart, oldStart + posFirstKeyframe);
		}

		// calculate the new width through the change of X
		// so that the right end stays where it is
		// otherwise the duration and not the right side would be snapped to grid
		const int deltaStart = oldStart - newStart;
		const int oldDuration = data.getSequenceDuration(sequenceData);
		const int newDuration = oldDuration + deltaStart;

		data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
		data.setSequenceDuration(sequenceData, newDuration);

		// move all keyframes, so the stay at their position
		const int numKeyframes = data.getNumKeyframes(sequenceData);
		for (int i = 0; i < numKeyframes; i++) {
			ValueTree keyframe = data.getKeyframe(sequenceData, i);
			const int keyframePosition = data.getKeyframePosition(keyframe);
			const int newKeyframePosition = keyframePosition + deltaStart;
			data.setKeyframePosition(keyframe, newKeyframePosition);
		}
	} else if (!xChanged && widthChanged) {
		// stretching right
		const int newWidth = newBounds.getWidth() / zoomFactor;
		const int start = data.getAbsoluteStartForSequence(sequenceData);
		const int newAbsoluteEnd = start + newWidth;
		const int newAbsoluteEndGrid = zoomFactor.snapValueToGrid(newAbsoluteEnd);
		int newDuration = newAbsoluteEndGrid - start;

		const int numKeyframes = data.getNumKeyframes(sequenceData);
		ValueTree lastKeyframe = data.getKeyframe(sequenceData, numKeyframes-1);
		if (lastKeyframe.isValid()) {
			const int posLastKeyframe = data.getKeyframePosition(lastKeyframe);
			newDuration = jmax(newDuration, posLastKeyframe);
		}

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

	for (int i = 0; i < numKeyframes; i++) {
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
	seqRect.removeFromBottom(1.0f);

	const bool selected = data.getSelection().contains(sequenceData);
	const String sequenceInterpolation = data.getSequenceInterpolation(sequenceData);

	if (nullptr == laf) {
		Colour fillColor = findColour(SequenceComponent::fillColourId);
		if (selected) {
			fillColor = findColour(SequenceComponent::highlightedFillColourId);
		}
		g.fillAll(fillColor);

		g.setColour(findColour(SequenceComponent::outlineColourId));
		g.drawRect(seqRect, 1);

		g.setColour(findColour(SequenceComponent::textColourId));
		g.drawFittedText(sequenceInterpolation, seqRect.getSmallestIntegerContainer(), Justification::centred, 1);
	} else {
		laf->drawSequence(g, seqRect, selected, sequenceInterpolation);
	}
}

void SequenceComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.originalComponent == &resizableBorder && m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		data.getUndoManager().beginNewTransaction("Resize Sequence");
		return;
	}

	if (m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		// drag sequence
		data.getUndoManager().beginNewTransaction("Drag Sequence");
		beginDragAutoRepeat(10); // time between drag events
		startDraggingComponent(this, event);
	} else if (m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier | ModifierKeys::shiftModifier)) {
		// copy sequence
		std::lock_guard<std::recursive_mutex> lock(data.getMutex());
		currentlyCopiedSequenceData = sequenceData.createCopy();
		data.getUndoManager().beginNewTransaction("Copy Sequence");
		data.addSequence(data.getSequenceParentUniform(sequenceData), currentlyCopiedSequenceData);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SequenceComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (!event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		// drag sequence
		dragComponent(this, event, &constrainer);

		// scroll viewport if necessary
		Viewport* parentViewport = findParentComponentOfClass<Viewport>();
		if (parentViewport == nullptr) {
			return;
		}
		const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
		// scroll only X- not Y-Direction
		// current X position gets normally set
		// current Y position is a constant that is greater than the minimum distance to the border (21 > 20)
		parentViewport->autoScroll(viewportEvent.x, 21, 20, 5);

	} else if (currentlyCopiedSequenceData.isValid()) {
		// copy sequence
		const int thisSequenceStart = data.getAbsoluteStartForSequence(sequenceData);
		const int mouseDistanceX = event.getDistanceFromDragStartX() / zoomFactor;
		const int newSequenceStart = jmax(0, thisSequenceStart + mouseDistanceX);
		const int newSequenceStartGrid = zoomFactor.snapValueToGrid(newSequenceStart);
		data.setSequencePropertiesForAbsoluteStart(currentlyCopiedSequenceData, newSequenceStartGrid);

	} else {
		McbComponent::mouseDrag(event);
	}
}

void SequenceComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.originalComponent == &resizableBorder && m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		return;
	}

	if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		// add keyframe
		const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
		const int sequenceDuration = data.getSequenceDuration(sequenceData);

		const int relativeMouseDown = event.getMouseDownX() / zoomFactor;
		const int absoluteMouseDown = relativeMouseDown + sequenceStart;

		const int absoluteMouseDownGrid = zoomFactor.snapValueToGrid(absoluteMouseDown);
		const int relativeMouseDownGrid = absoluteMouseDownGrid - sequenceStart;

		if (relativeMouseDownGrid >= 0 && relativeMouseDownGrid <= sequenceDuration) {
			// don't set keyframe beyond start or end

			if (! data.getKeyframe(sequenceData, var(relativeMouseDownGrid)).isValid()) {
				// check if there's already a keyframe with this position
				data.getUndoManager().beginNewTransaction("Create Keyframe");
				data.addKeyframe(sequenceData, relativeMouseDownGrid);
			}
		}

	} else if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::popupMenuClickModifier | ModifierKeys::commandModifier)) {
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

	} else if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::middleButtonModifier | ModifierKeys::commandModifier)) {
		// delete sequence
		data.getSelection().remove(sequenceData);
		data.getUndoManager().beginNewTransaction("Remove Sequence");
		data.removeSequence(sequenceData);
		// this component gets deleted after this, so don't do stupid things

	} else if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::rightButtonModifier)) {
		// set sequence as selection
		data.getSelection().set(sequenceData);

	} else if (event.mouseWasClicked() && m == ModifierKeys(ModifierKeys::rightButtonModifier | ModifierKeys::shiftModifier)) {
		// add sequence to selection
		data.getSelection().toggle(sequenceData);

	} else if (currentlyCopiedSequenceData.isValid()) {
		// end of sequence copying
		if (data.getAbsoluteStartForSequence(sequenceData) == data.getAbsoluteStartForSequence(currentlyCopiedSequenceData)) {
			// copied sequence at same position as this one
			data.removeSequence(currentlyCopiedSequenceData);
			data.getUndoManager().undoCurrentTransactionOnly();
		}
		currentlyCopiedSequenceData = ValueTree();

	} else {
		McbComponent::mouseUp(event);
	}
}

void SequenceComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());
}

void SequenceComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &zoomFactor) {
		// zoomFactor update
		constrainer.setMinimumWidth(zoomFactor.getGridWidth() * zoomFactor);
		updateBounds();
	} else if (source == &data.getSelection()) {
		// selection update
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
		addKeyframeComponent(childWhichHasBeenAdded);
	} else if (childWhichHasBeenAdded.hasType(treeId::uniform)) {
		// uniform got inserted, row number may changed
		updateBounds();
	}
}

void SequenceComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int /*indexFromWhichChildWasRemoved*/) {
	// TODO: react on removal of scenes
	if (data.getKeyframesArray(sequenceData) == parentTree) {
		KeyframeComponent* keyframeComponent = getKeyframeComponentForData(childWhichHasBeenRemoved);
		keyframeComponentsArray.removeObject(keyframeComponent);
	} else if (childWhichHasBeenRemoved.hasType(treeId::uniform)) {
		// uniform got inserted, row number may changed
		updateBounds();
	}
}

void SequenceComponent::valueTreeChildOrderChanged(ValueTree& parentTree, int /*oldIndex*/, int /*newIndex*/) {
	if (parentTree == data.getUniformsArray()) {
		// uniforms order changed, row number may changed
		updateBounds();
	}
}

void SequenceComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

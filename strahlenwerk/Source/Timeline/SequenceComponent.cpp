#include "SequenceComponent.h"

#include "TimelineData.h"
#include "KeyframeComponent.h"
#include "SequenceViewComponent.h"

SequenceComponent::SequenceComponent(ValueTree _sequenceData, ZoomFactor& zoomFactor_, int y, int height) :
	sequenceData(_sequenceData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	// initialize the value pointing to the start time of the scene this sequence belongs to
	updateSceneStartValueRefer();
	sceneStartValue.addListener(this);

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
	updateKeyframeComponents();
}

void SequenceComponent::valueChanged(Value& /*value*/) {
	updateBounds();
}

void SequenceComponent::updateBounds() {
	const float start = (float) data.getAbsoluteStartForSequence(sequenceData) * zoomFactor;
	const float duration = (float) data.getSequenceDuration(sequenceData) * zoomFactor;

	Rectangle<int> bounds = getBounds();
	bounds.setX(start);
	bounds.setWidth(duration);
	setBounds(bounds);
}

// update the value sceneStartValue refers to
void SequenceComponent::updateSceneStartValueRefer() {
	const var sceneId = data.getSequenceSceneId(sequenceData);
	ValueTree sceneForSequence = data.getScene(sceneId);
	// TODO: remove this sceneStartValue completly or add a get*AsValue function
	sceneStartValue.referTo(sceneForSequence.getPropertyAsValue("sceneStart", nullptr));
}

void SequenceComponent::updateKeyframeComponents() {
	keyframeComponentsArray.clearQuick(true);
	const int numKeyframes = data.getNumKeyframes(sequenceData);

	// don't create a component for first and last keyframe
	for (int i = 1; i < numKeyframes - 1; i++) {
		ValueTree keyframeData = data.getKeyframe(sequenceData, i);
		auto keyframeComponent = new KeyframeComponent(keyframeData, zoomFactor);
		addAndMakeVisible(keyframeComponent);
		keyframeComponentsArray.add(keyframeComponent);
	}
}

void SequenceComponent::removeKeyframeComponent(const KeyframeComponent* toBeDeleted) {
	keyframeComponentsArray.removeObject(toBeDeleted);
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
	beginDragAutoRepeat(10); // time between drag events
	startDraggingComponent(this, event);
}

void SequenceComponent::mouseDrag(const MouseEvent& event) {
	if (event.mouseWasClicked()) {
		return;
	}

	dragComponent(this, event, &constrainer);

	// scroll viewport if necessary
	Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
	// scroll only X- not Y-Direction
	// current X position gets normally set
	// current Y position is a constant that is greater than the minimum distance to the border (21 > 20)
	parentViewport->autoScroll(viewportEvent.x, 21, 20, 5);
}

void SequenceComponent::mouseUp(const MouseEvent& event) {
	if (!event.mouseWasClicked()) {
		// do nothing on drag
		return;
	}

	const ModifierKeys& mods = event.mods;

	if (mods.isLeftButtonDown()) {
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

	} else if (mods.isMiddleButtonDown() && mods.isCtrlDown()) {
		// delete sequence
		AlertWindow reallyDeleteWindow("Sequence", L"Delëte this Sequence for a Long Time?", AlertWindow::WarningIcon);
		reallyDeleteWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));
		reallyDeleteWindow.addButton("Delete", 1, KeyPress('d'), KeyPress(KeyPress::spaceKey));

		const int returnedChoice = reallyDeleteWindow.runModalLoop();
		if (returnedChoice != 1) {
			return;
		}

		data.removeSequence(sequenceData);
		findParentComponentOfClass<SequenceViewComponent>()->removeSequenceComponent(this);
	}
}

void SequenceComponent::moved() {
	// update the sceneId and relativ start time
	const int newStart = constrainer.snapValueToGrid(getX() / zoomFactor);
	data.setSequencePropertiesForAbsoluteStart(sequenceData, newStart);
	updateSceneStartValueRefer();
}

void SequenceComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());

	const int newDuration = constrainer.snapValueToGrid(getWidth() / zoomFactor);
	data.setSequenceDuration(sequenceData, newDuration);
}

void SequenceComponent::zoomFactorChanged(ZoomFactor&) {
	updateBounds();
}

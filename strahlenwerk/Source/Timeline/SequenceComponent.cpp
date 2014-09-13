#include "SequenceComponent.h"

#include "TreeIdentifiers.h"
#include "Timeline.h"
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
	const var sceneId = sequenceData.getProperty(treeId::sequenceSceneId);
	ValueTree sceneForSequence = data.getScenesArray().getChildWithProperty(treeId::sceneId, sceneId);
	sceneStartValue.referTo(sceneForSequence.getPropertyAsValue(treeId::sceneStart, nullptr));
}

void SequenceComponent::updateKeyframeComponents() {
	keyframeComponentsArray.clearQuick(true);
	ValueTree keyframeDatasArray = data.getKeyframesArray(sequenceData);
	const int numKeyframes = keyframeDatasArray.getNumChildren();

	// don't create a component for first and last keyframe
	for (int i = 1; i < numKeyframes - 1; i++) {
		ValueTree keyframeData = keyframeDatasArray.getChild(i);
		KeyframeComponent* keyframeComponent = new KeyframeComponent(keyframeData);
		addAndMakeVisible(keyframeComponent);
		keyframeComponent->updateBounds();
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
	Timeline::ViewportCallback* parentViewport = findParentComponentOfClass<Timeline::ViewportCallback>();
	const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
	Point<int> currentPos = viewportEvent.getPosition();
	// scroll only X- not Y-Direction, so set it to something > 20
	parentViewport->autoScroll(currentPos.getX(), 21, 20, 5);
}

void SequenceComponent::mouseUp(const MouseEvent& event) {
	if (!event.mouseWasClicked()) {
		return;
	}

	const ModifierKeys& mods = event.mods;

	if (mods.isLeftButtonDown()) {
		// add keyframe
		const int gridWidth = 20;

		const int mouseDown = event.getMouseDownX();
		const int mouseDownGrid = roundFloatToInt(float(mouseDown) / float(gridWidth)) * gridWidth;

		const int sequenceDuration = sequenceData.getProperty(treeId::sequenceDuration);
		if (mouseDownGrid == 0 || mouseDownGrid == sequenceDuration) {
			// don't set keyframe at start or end
			return;
		}

		data.addKeyframe(sequenceData, var(mouseDownGrid));
		ValueTree keyframeData = data.getKeyframesArray(sequenceData).getChildWithProperty(treeId::keyframePosition, var(mouseDownGrid));

		KeyframeComponent* keyframeComponent = new KeyframeComponent(keyframeData);
		addAndMakeVisible(keyframeComponent);
		keyframeComponent->updateBounds();
		keyframeComponentsArray.add(keyframeComponent);
	} else if (mods.isMiddleButtonDown() && mods.isCtrlDown()) {
		AlertWindow reallyDeleteWindow("Sequence", L"Delëte this Sequence for Ever and Ever", AlertWindow::WarningIcon);
		reallyDeleteWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));
		reallyDeleteWindow.addButton("Delete", 1, KeyPress('d'), KeyPress(KeyPress::spaceKey));

		const int returnedChoice = reallyDeleteWindow.runModalLoop();
		if (returnedChoice != 1) {
			return;
		}

		sequenceData.getParent().removeChild(sequenceData, nullptr);
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

	ValueTree keyframesArray = data.getKeyframesArray(sequenceData);
	ValueTree lastKeyframe = keyframesArray.getChild(keyframesArray.getNumChildren() - 1);
	lastKeyframe.setProperty(treeId::keyframePosition, newDuration, nullptr);
}

void SequenceComponent::zoomFactorChanged(ZoomFactor&) {
	updateBounds();
}

#include "SceneComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"

SceneComponent::SceneComponent(ValueTree _sceneData, ZoomFactor& zoomFactor_) :
	sceneData(_sceneData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	const int gridWidth = 20;

	Value shaderSourceValue = data.getSceneShaderSourceAsValue(sceneData);
	shaderSourceLabel.getTextValue().referTo(shaderSourceValue);
	shaderSourceLabel.setEditable(false, true, false);
	shaderSourceLabel.setJustificationType(Justification::centred);
	//shaderSourceLabel.setColour(Label::textColourId, findColour(SceneComponent::textColourId)); // TODO: find out why this throws an assertion
	addAndMakeVisible(shaderSourceLabel);

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(gridWidth);
	constrainer.setMinimumWidth(gridWidth);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	setMouseCursor(MouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor));
	sceneData.addListener(this);
	zoomFactor.addListener(this);
}

SceneComponent::~SceneComponent() {
	sceneData.removeListener(this);
	zoomFactor.removeListener(this);
}

void SceneComponent::updateBounds() {
	const float start = (float) data.getSceneStart(sceneData) * zoomFactor;
	const float duration = (float) data.getSceneDuration(sceneData) * zoomFactor;

	const float padding = 0;
	setBounds(start, padding, duration, getParentHeight() - 2*padding);
}

void SceneComponent::paint(Graphics& g) {
	const float cornerSize = 5.0;
	const float outlineThickness = 1.;

	Rectangle<float> rect = getLocalBounds().toFloat();
	rect.removeFromTop(.5);
	rect.removeFromBottom(1.5);

	g.setColour(findColour(SceneComponent::fillColourId));
	g.fillRoundedRectangle(rect, cornerSize);

	g.setColour(findColour(SceneComponent::outlineColourId));
	g.drawRoundedRectangle(rect, cornerSize, outlineThickness);
}

void SceneComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown()) {
		beginDragAutoRepeat(10); // time between drag events
		startDraggingComponent(this, event);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SceneComponent::mouseDrag(const MouseEvent& event) {
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

void SceneComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.mouseWasClicked() && m.isMiddleButtonDown() && m.isCommandDown()) {
		AlertWindow reallyDeleteWindow("Scene", "Delete this Scene for a Long Time", AlertWindow::WarningIcon);
		reallyDeleteWindow.addButton("Cancel", 0, KeyPress('c'), KeyPress(KeyPress::escapeKey));
		reallyDeleteWindow.addButton("Maybe", 1, KeyPress('m'));
		reallyDeleteWindow.addButton("Delete", 2, KeyPress('d'), KeyPress(KeyPress::spaceKey));

		const int returnedChoice = reallyDeleteWindow.runModalLoop();
		if (returnedChoice != 2) {
			Random random;
			if (returnedChoice == 1 && random.nextBool()) {
				// 50% chance to remove the scene
			} else {
				return;
			}
		}

		data.removeScene(sceneData);
		// this component gets deleted after this, so don't do stupid things
	} else {
		McbComponent::mouseUp(event);
	}
}

void SceneComponent::moved() {
	const int newStart = constrainer.snapValueToGrid(getX() / zoomFactor);
	data.setSceneStart(sceneData, newStart);
}

void SceneComponent::resized() {
	shaderSourceLabel.setBounds(getLocalBounds());
	resizableBorder.setBounds(getLocalBounds());
	const int newDuration = constrainer.snapValueToGrid(getWidth() / zoomFactor);
	data.setSceneDuration(sceneData, newDuration);
}

void SceneComponent::parentHierarchyChanged() {
	updateBounds();
}

void SceneComponent::zoomFactorChanged(ZoomFactor&) {
	updateBounds();
}

// ValueTree::Listener callbacks
void SceneComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& property) {
	// callback comes only from the sceneData that belongs to this component
	if (property == treeId::sceneStart || property == treeId::sceneDuration) {
		updateBounds();
	}
}

void SceneComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
}

void SceneComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/) {
}

void SceneComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void SceneComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

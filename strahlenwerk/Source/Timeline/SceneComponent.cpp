#include "SceneComponent.h"
#include "ScenesBarComponent.h"
#include "TimelineData.h"

SceneComponent::SceneComponent(ValueTree _sceneData, ZoomFactor& zoomFactor_) :
	sceneData(_sceneData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	const int gridWidth = 20;

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(gridWidth);
	constrainer.setMinimumWidth(gridWidth);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	setMouseCursor(MouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor));
	zoomFactor.addListener(this);
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

	String shaderSourceName = data.getSceneShaderSource(sceneData);
	g.setColour(findColour(SceneComponent::textColourId));
	g.drawText(shaderSourceName, rect, Justification(Justification::centred), true);
}

void SceneComponent::mouseDown(const MouseEvent& event) {
	beginDragAutoRepeat(10); // time between drag events
	startDraggingComponent(this, event);
}

void SceneComponent::mouseDrag(const MouseEvent& event) {
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

void SceneComponent::mouseUp(const MouseEvent& event) {
	if (!event.mouseWasClicked()) {
		return;
	}

	const ModifierKeys& mods = event.mods;
	if (mods.isMiddleButtonDown() && mods.isCtrlDown()) {
		AlertWindow reallyDeleteWindow("Scene", "Delete this Scene for Ever and Ever", AlertWindow::WarningIcon);
		reallyDeleteWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));
		reallyDeleteWindow.addButton("Delete", 1, KeyPress('d'), KeyPress(KeyPress::spaceKey));

		const int returnedChoice = reallyDeleteWindow.runModalLoop();
		if (returnedChoice != 1) {
			return;
		}

		data.removeScene(sceneData);
		findParentComponentOfClass<ScenesBarComponent>()->removeSceneComponent(this);
	}
}

void SceneComponent::moved() {
	const int newStart = constrainer.snapValueToGrid(getX() / zoomFactor);
	data.setSceneStart(sceneData, newStart);
}

void SceneComponent::resized() {
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

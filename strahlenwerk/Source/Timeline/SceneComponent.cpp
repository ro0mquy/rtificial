#include "SceneComponent.h"
#include "TreeIdentifiers.h"
#include "Timeline.h"
#include "ScenesBarComponent.h"
#include "TimelineData.h"
#include "ZoomFactor.h"

SceneComponent::SceneComponent(ValueTree _sceneData, ZoomFactor& zoomFactor_) :
	sceneData(_sceneData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(20);
	constrainer.setMinimumWidth(20);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	setMouseCursor(MouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor));
}

void SceneComponent::updateBounds() {
	const float start = (float) data.getSceneStart(sceneData) * zoomFactor;
	const float duration = (float) data.getSceneDuration(sceneData) * zoomFactor;

	const float padding = 0;
	setBounds(start, padding, duration, getParentHeight() - 2*padding);
}

void SceneComponent::paint(Graphics& g) {
	const float cornerSize = 5.0;
	Rectangle<float> rect = getLocalBounds().toFloat();
	rect.removeFromTop(.5);
	rect.removeFromBottom(1.5);

	String shaderSourceName = sceneData.getProperty(treeId::sceneShaderSource);
	g.setColour(findColour(SceneComponent::fillColourId));
	g.fillRoundedRectangle(rect, cornerSize);

	g.setColour(findColour(SceneComponent::outlineColourId));
	g.drawRoundedRectangle(rect, cornerSize, 1);

	g.setColour(findColour(SceneComponent::textColourId));
	g.drawText(shaderSourceName, rect, Justification(Justification::centred), true);
}

void SceneComponent::mouseDown(const MouseEvent& event) {
	beginDragAutoRepeat(10); // time between drag events
	startDraggingComponent(this, event);
}

void SceneComponent::mouseDrag(const MouseEvent& event) {
	dragComponent(this, event, &constrainer);

	// scroll viewport if necessary
	Timeline::ViewportCallback* parentViewport = findParentComponentOfClass<Timeline::ViewportCallback>();
	const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
	Point<int> currentPos = viewportEvent.getPosition();
	// scroll only X- not Y-Direction, so set it to something > 20
	parentViewport->autoScroll(currentPos.getX(), 21, 20, 5);
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

		sceneData.getParent().removeChild(sceneData, nullptr);
		findParentComponentOfClass<ScenesBarComponent>()->removeSceneComponent(this);
	}
}

void SceneComponent::moved() {
	sceneData.setProperty(treeId::sceneStart, var(getX()), nullptr);
}

void SceneComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());
	sceneData.setProperty(treeId::sceneDuration, var(getWidth()), nullptr);
}

void SceneComponent::parentHierarchyChanged() {
	updateBounds();
}

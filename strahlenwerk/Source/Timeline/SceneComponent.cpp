#include "SceneComponent.h"
#include "TreeIdentifiers.h"
#include "Timeline.h"

SceneComponent::SceneComponent(ValueTree _sceneData) :
	sceneData(_sceneData),
	resizableBorder(this, &constrainer)
{
	// don't drag over the component edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setGridWidth(20);
	constrainer.setMinimumWidth(20);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	setMouseCursor(MouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor));

	// start timer, so the bounds get set,
	// when this component was added to the ScenesBarComponent
	startTimer(0);
}

void SceneComponent::timerCallback() {
	stopTimer();

	// set bounds
	const int start = sceneData.getProperty(treeId::sceneStart);
	const int duration = sceneData.getProperty(treeId::sceneDuration);

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
	beginDragAutoRepeat(100); // time between drag events
	startDraggingComponent(this, event);
}

void SceneComponent::mouseDrag(const MouseEvent& event) {
	dragComponent(this, event, &constrainer);

	// scroll viewport if necessary
	Timeline::ViewportCallback* parentViewport = findParentComponentOfClass<Timeline::ViewportCallback>();
	const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
	Point<int> currentPos = viewportEvent.getPosition();
	parentViewport->autoScroll(currentPos.getX(), currentPos.getY(), 20, 5);
}

void SceneComponent::moved() {
	sceneData.setProperty(treeId::sceneStart, var(getX()), nullptr);
}

void SceneComponent::resized() {
	resizableBorder.setBounds(getLocalBounds());
	sceneData.setProperty(treeId::sceneDuration, var(getWidth()), nullptr);
}

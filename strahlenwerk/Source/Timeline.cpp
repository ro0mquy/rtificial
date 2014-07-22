#include "Timeline.h"

// make a callback to Timeline if one of the viewports changed
void ViewportCallback::visibleAreaChanged(const Rectangle<int>& newVisibleArea) {
	Timeline* timeline = findParentComponentOfClass<Timeline>();
	timeline->callbackViewportChanged(this, newVisibleArea.getPosition());
};

TimelineCanvas::TimelineCanvas() : button("Hello World!")
{
	setSize(1000, 1000);
	addAndMakeVisible(button);
}

void TimelineCanvas::resized() {
	button.setBounds(getLocalBounds().reduced(250));
}

// functions of the allmighty Timeline class
Timeline::Timeline()
{
	viewportCanvas.setViewedComponent(&componentCanvas, false);
	viewportScenes.setViewedComponent(&componentScenes, false);
	viewportUniforms.setViewedComponent(&componentUniforms, false);
	addAndMakeVisible(viewportCanvas);
	addAndMakeVisible(viewportScenes);
	addAndMakeVisible(viewportUniforms);
}

void Timeline::resized() {
	Rectangle<int> r(getLocalBounds());
	viewportScenes.setBounds(r.removeFromTop(30).withTrimmedLeft(30));
	viewportUniforms.setBounds(r.removeFromLeft(30));
	viewportCanvas.setBounds(r);
}

// gets called when one of the viewports changed
// syncs the current position between all three viewports
void Timeline::callbackViewportChanged(ViewportCallback* vp, Point<int> position) {
	if (vp == &viewportCanvas) {
		// X and Y have changed
		viewportScenes.setViewPosition(
				position.getX(),
				viewportScenes.getViewPositionY()
				);
		viewportUniforms.setViewPosition(
				viewportUniforms.getViewPositionX(),
				position.getY()
				);
	} else if (vp == &viewportScenes) {
		// only X has changed
		viewportCanvas.setViewPosition(
				position.getX(),
				viewportCanvas.getViewPositionY()
				);
	} else if (vp == &viewportUniforms) {
		// only Y has changed
		viewportCanvas.setViewPosition(
				viewportCanvas.getViewPositionX(),
				position.getY()
				);
	}
}

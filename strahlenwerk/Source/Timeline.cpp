#include "Timeline.h"

Timeline::ViewportCallback::ViewportCallback(Timeline& timelineParent) :
	timelineParent(timelineParent)
{
}

// make a callback to Timeline if one of the viewports changed
void Timeline::ViewportCallback::visibleAreaChanged(const Rectangle<int>& newVisibleArea) {
	timelineParent.callbackViewportChanged(this, newVisibleArea.getPosition());
};

// functions of the allmighty Timeline class
Timeline::Timeline() :
	viewportCanvas(*this),
	viewportScenes(*this),
	viewportUniforms(*this)
{
	viewportCanvas.setViewedComponent(&componentCanvas, false);
	viewportScenes.setViewedComponent(&componentScenes, false);
	viewportScenes.setScrollBarsShown(false, false, false, true);
	viewportUniforms.setViewedComponent(&componentUniforms, false);
	viewportUniforms.setScrollBarsShown(false, false, true, false);
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
void Timeline::callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position) {
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

#include "Timeline.h"

// functions of the allmighty Timeline class
Timeline::Timeline() :
	scenesBar(zoomFactor)
{
	viewportSequenceView.setViewedComponent(&sequenceView, false);
	viewportScenesBar.setViewedComponent(&scenesBar, false);
	viewportScenesBar.setScrollBarsShown(false, false, false, true);
	viewportUniformsBar.setViewedComponent(&uniformsBar, false);
	viewportUniformsBar.setScrollBarsShown(false, false, true, false);
	addAndMakeVisible(viewportSequenceView);
	addAndMakeVisible(viewportScenesBar);
	addAndMakeVisible(viewportUniformsBar);
}

void Timeline::resized() {
	const int scenesBarHeight = 30;
	const int uniformsBarWidth = 150;
	Rectangle<int> r(getLocalBounds());

	viewportScenesBar.setBounds(
			r.removeFromTop(scenesBarHeight)
			.withTrimmedLeft(uniformsBarWidth)
		);
	scenesBar.updateSize();

	viewportUniformsBar.setBounds(r.removeFromLeft(uniformsBarWidth));
	uniformsBar.setSize(
			uniformsBarWidth,
			uniformsBar.getHeight()
		);
	uniformsBar.updateSize();

	viewportSequenceView.setBounds(r);
	sequenceView.updateSize();
}

void Timeline::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	if (!event.mods.isCtrlDown()) {
		Component::mouseWheelMove(event, wheel);
	}

	zoomFactor *= exp(wheel.deltaY);
}

ZoomFactor& Timeline::getZoomFactor() {
	return zoomFactor;
}

// gets called when one of the viewports changed
// syncs the current position between all three viewports
void Timeline::callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position) {
	if (vp == &viewportSequenceView) {
		// X and Y have changed
		viewportScenesBar.setViewPosition(
				position.getX(),
				viewportScenesBar.getViewPositionY()
				);
		viewportUniformsBar.setViewPosition(
				viewportUniformsBar.getViewPositionX(),
				position.getY()
				);
	} else if (vp == &viewportScenesBar) {
		// only X has changed
		viewportSequenceView.setViewPosition(
				position.getX(),
				viewportSequenceView.getViewPositionY()
				);
	} else if (vp == &viewportUniformsBar) {
		// only Y has changed
		viewportSequenceView.setViewPosition(
				viewportSequenceView.getViewPositionX(),
				position.getY()
				);
	}
}

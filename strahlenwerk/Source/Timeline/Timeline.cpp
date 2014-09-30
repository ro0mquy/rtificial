#include "Timeline.h"
#include "TimelineData.h"
#include "SpecialUniformController.h"
#include <AudioManager.h>

// functions of the allmighty Timeline class
Timeline::Timeline() :
	sequenceView(zoomFactor),
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
	uniformsBar.updateSize();

	viewportSequenceView.setBounds(r);
	sequenceView.updateSize();
}

void Timeline::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	const float posXInViewport = event.getEventRelativeTo(&viewportSequenceView).position.x;
	if (posXInViewport >= 0 && event.mods.isCommandDown()) {
		const float scaleFactor = exp(wheel.deltaY);
		zoomFactor *= scaleFactor;
		Point<int> position = viewportSequenceView.getViewPosition();
		float zoomCenterPosition = position.x;
		if (scaleFactor > 1.) {
			// zoom in
			// zoom around mouse position
			zoomCenterPosition = event.getEventRelativeTo(&sequenceView).position.x;
		} else {
			// zoom out
			// zoom around middle point
			zoomCenterPosition = position.x + viewportSequenceView.getMaximumVisibleWidth() / 2.;
		}
		position.x += (scaleFactor - 1.) * zoomCenterPosition;
		viewportSequenceView.setViewPosition(position);
	} else {
		Component::mouseWheelMove(event, wheel);
	}
}

void Timeline::mouseDown(const MouseEvent& event) {
	const int uniformsBarWidth = 150;
	const bool isInUniBar = event.getMouseDownX() < uniformsBarWidth;
	const ModifierKeys& m = event.mods;
	if (!isInUniBar && m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		// set current Time
		mouseDrag(event);
	} else {
		Component::mouseDown(event);
	}
}

void Timeline::mouseDrag(const MouseEvent& event) {
	const int uniformsBarWidth = 150;
	const bool isInUniBar = event.getMouseDownX() < uniformsBarWidth;
	const ModifierKeys& m = event.mods;
	if (!isInUniBar && m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		if (CameraController::globalCameraController) {
			if (m.isShiftDown()) {
				CameraController::globalCameraController->releaseControl();
			} else {
				CameraController::globalCameraController->takeOverControl();
			}
		}

		const MouseEvent eventSeqView = event.getEventRelativeTo(&sequenceView);
		const float newTime = jmax(eventSeqView.x, 0) / zoomFactor;
		const float newTimeSnapped = zoomFactor.snapValueToGrid(newTime);
		AudioManager::getAudioManager().setTime(newTimeSnapped);
	} else {
		Component::mouseDrag(event);
	}
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

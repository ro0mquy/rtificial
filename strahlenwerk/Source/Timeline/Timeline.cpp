#include "Timeline.h"
#include "TimelineData.h"
#include "SpecialUniformController.h"
#include <AudioManager.h>

// functions of the allmighty Timeline class
Timeline::Timeline() :
	sequenceView(zoomFactor),
	scenesBar(zoomFactor),
	scenesBarSequenceViewBoxLayout(scenesBarSequenceViewLayout, {{ &viewportScenesBar, &viewportSequenceView }}),
	uniformsBarResizer(&timelineLayout, 1, true),
	timelineBoxLayout(timelineLayout, {{ &viewportUniformsBar, &uniformsBarResizer, &scenesBarSequenceViewBoxLayout }})
{
	viewportSequenceView.setViewedComponent(&sequenceView, false);
	viewportScenesBar.setViewedComponent(&scenesBar, false);
	viewportScenesBar.setScrollBarsShown(false, false, false, true);
	viewportUniformsBar.setViewedComponent(&uniformsBar, false);
	viewportUniformsBar.setScrollBarsShown(false, false, true, false);

	scenesBarSequenceViewLayout.setItemLayout(0, 30, 30, 30); // scenesBar
	scenesBarSequenceViewLayout.setItemLayout(1, 170, -1., -1.); // sequenceView

	timelineLayout.setItemLayout(0, 100, -1., 150); // uniformsBar
	timelineLayout.setItemLayout(1, 8, 8, 8); // uniformsBarResizer
	timelineLayout.setItemLayout(2, 200, -1., 650); // scenesBar & sequenceView

	scenesBarSequenceViewBoxLayout.setInterceptsMouseClicks(false, true);
	timelineBoxLayout.setInterceptsMouseClicks(false, true);
	addAndMakeVisible(timelineBoxLayout);
}

void Timeline::resized() {
	const int scenesBarHeight = 30;

	// the timelineBoxLayout doesn't update the components height
	// so the uniformsBar could be made somewhat smaller
	viewportUniformsBar.setBounds(viewportUniformsBar.getX(), scenesBarHeight, viewportUniformsBar.getWidth(), getHeight() - scenesBarHeight);
	uniformsBarResizer.setSize(uniformsBarResizer.getWidth(), getHeight());
	scenesBarSequenceViewBoxLayout.setSize(scenesBarSequenceViewBoxLayout.getWidth(), getHeight());

	timelineBoxLayout.setBounds(getLocalBounds());
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
	const MouseEvent eventViewportSeqView = event.getEventRelativeTo(&viewportSequenceView);
	const bool isNotInUniBar = eventViewportSeqView.getMouseDownX() >= 0;

	const ModifierKeys& m = event.mods;
	if (isNotInUniBar && m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		// set current Time
		mouseDrag(event);
	} else {
		Component::mouseDown(event);
	}
}

void Timeline::mouseDrag(const MouseEvent& event) {
	const MouseEvent eventViewportSeqView = event.getEventRelativeTo(&viewportSequenceView);
	const bool isNotInUniBar = eventViewportSeqView.getMouseDownX() >= 0;

	const ModifierKeys& m = event.mods;
	if (isNotInUniBar && m.isLeftButtonDown() && (m.isShiftDown() || !m.isAnyModifierKeyDown())) {
		if (CameraController::globalCameraController != nullptr) {
			if (m.isShiftDown()) {
				CameraController::globalCameraController->releaseControl();
			} else {
				CameraController::globalCameraController->takeOverControl();
			}
		}

		const MouseEvent eventSeqView = event.getEventRelativeTo(&sequenceView);
		const int newTime = jmax(eventSeqView.x, 0) / zoomFactor;
		const int newTimeSnapped = zoomFactor.snapValueToGrid(newTime);
		AudioManager::getAudioManager().setTime(newTimeSnapped);
	} else {
		Component::mouseDrag(event);
	}
}

void Timeline::mouseUp(const MouseEvent& event) {
	const MouseEvent eventViewportSeqView = event.getEventRelativeTo(&viewportSequenceView);
	const bool isNotInUniBar = eventViewportSeqView.getMouseDownX() >= 0 && eventViewportSeqView.x >= 0;

	const ModifierKeys& m = event.mods;
	if (isNotInUniBar && m.isRightButtonDown() && m.isShiftDown()) {
		// clear selection
		TimelineData::getTimelineData().getSelection().clear();
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

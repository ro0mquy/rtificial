#include "ZoomFactor.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <Timeline/Timeline.h>

ZoomFactor::ZoomFactor() :
	zoomLevel(1.)
{
}

ZoomFactor& ZoomFactor::getZoomFactor() {
	return StrahlenwerkApplication::getInstance()->getMainWindow().getMainContentComponent().getTimeline().getZoomFactor();
}

ZoomFactor::operator float() {
	zoomMutex.lock();
	const float tmpZoomFactor = zoomLevel;
	zoomMutex.unlock();
	return tmpZoomFactor;
}

ZoomFactor& ZoomFactor::operator=(const float newZoomLevel) {
	zoomMutex.lock();
	zoomLevel = newZoomLevel;
	zoomMutex.unlock();
	return *this;
}

ZoomFactor& ZoomFactor::operator*=(const float zoomLevelFactor) {
	zoomMutex.lock();
	zoomLevel *= zoomLevelFactor;
	zoomMutex.unlock();
	return *this;
}

ZoomFactor& ZoomFactor::operator/=(const float zoomLevelDivisor) {
	zoomMutex.lock();
	zoomLevel /= zoomLevelDivisor;
	zoomMutex.unlock();
	return *this;
}

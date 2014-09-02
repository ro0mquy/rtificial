#include "Zoom.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <Timeline/Timeline.h>

Zoom::Zoom() :
	zoomLevel(1.)
{
}

Zoom& Zoom::getZoom() {
	return StrahlenwerkApplication::getInstance()->getMainWindow().getMainContentComponent().getTimeline().getZoom();
}

Zoom::operator float() {
	zoomMutex.lock();
	const float tmpZoom = zoomLevel;
	zoomMutex.unlock();
	return tmpZoom;
}

Zoom& Zoom::operator=(const float newZoomLevel) {
	zoomMutex.lock();
	zoomLevel = newZoomLevel;
	zoomMutex.unlock();
	return *this;
}

Zoom& Zoom::operator*=(const float zoomLevelFactor) {
	zoomMutex.lock();
	zoomLevel *= zoomLevelFactor;
	zoomMutex.unlock();
	return *this;
}

Zoom& Zoom::operator/=(const float zoomLevelDivisor) {
	zoomMutex.lock();
	zoomLevel /= zoomLevelDivisor;
	zoomMutex.unlock();
	return *this;
}

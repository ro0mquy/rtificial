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

// multiply with ZoomFactor to convert time into pixels
// divide with ZoomFactor to convert pixels into time
ZoomFactor::operator float() {
	std::lock_guard<std::mutex> lock(zoomMutex);
	return zoomLevel;
}

ZoomFactor& ZoomFactor::operator=(const float newZoomLevel) {
	zoomMutex.lock();
	const float oldZoomLevel = zoomLevel;
	zoomLevel = jmax(newZoomLevel, .1f); // without max() there will be dragons
	const float tmpZoomLevel = zoomLevel;
	zoomMutex.unlock();
	if (tmpZoomLevel != oldZoomLevel) {
		listeners.call(&ZoomFactor::Listener::zoomFactorChanged, *this);
	}
	return *this;
}

ZoomFactor& ZoomFactor::operator*=(const float zoomLevelFactor) {
	zoomMutex.lock();
	const float oldZoomLevel = zoomLevel;
	zoomLevel *= zoomLevelFactor;
	zoomLevel = jmax(zoomLevel, .1f); // without max() there will be dragons
	const float tmpZoomLevel = zoomLevel;
	zoomMutex.unlock();
	if (tmpZoomLevel != oldZoomLevel) {
		listeners.call(&ZoomFactor::Listener::zoomFactorChanged, *this);
	}
	return *this;
}

ZoomFactor& ZoomFactor::operator/=(const float zoomLevelDivisor) {
	zoomMutex.lock();
	const float oldZoomLevel = zoomLevel;
	zoomLevel /= zoomLevelDivisor;
	zoomLevel = jmax(zoomLevel, .1f); // without max() there will be dragons
	const float tmpZoomLevel = zoomLevel;
	zoomMutex.unlock();
	if (tmpZoomLevel != oldZoomLevel) {
		listeners.call(&ZoomFactor::Listener::zoomFactorChanged, *this);
	}
	return *this;
}

float ZoomFactor::timeToPixels(const float time) {
	std::lock_guard<std::mutex> lock(zoomMutex);
	return time * zoomLevel;
}

float ZoomFactor::pixelsToTime(const float pixels) {
	std::lock_guard<std::mutex> lock(zoomMutex);
	return pixels / zoomLevel;
}

void ZoomFactor::addListener(Listener* const listener) {
	listeners.add(listener);
}

void ZoomFactor::removeListener(Listener* const listener) {
	listeners.remove(listener);
}

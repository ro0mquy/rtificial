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
	zoomLevel = newZoomLevel;
	zoomMutex.unlock();
	listeners.call(&ZoomFactor::Listener::zoomFactorChanged, *this);
	return *this;
}

ZoomFactor& ZoomFactor::operator*=(const float zoomLevelFactor) {
	return operator=(zoomLevel * zoomLevelFactor);
}

ZoomFactor& ZoomFactor::operator/=(const float zoomLevelDivisor) {
	return operator=(zoomLevel / zoomLevelDivisor);
}

float ZoomFactor::timeToPixels(const float time) {
	return time * *this;
}

float ZoomFactor::pixelsToTime(const float pixels) {
	return pixels / *this;
}

void ZoomFactor::addListener(Listener* const listener) {
	listeners.add(listener);
}

void ZoomFactor::removeListener(Listener* const listener) {
	listeners.remove(listener);
}

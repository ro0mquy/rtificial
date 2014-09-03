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
	listeners.call(&ZoomFactor::Listener::zoomFactorChanged, *this);
	return *this;
}

ZoomFactor& ZoomFactor::operator*=(const float zoomLevelFactor) {
	return operator=(zoomLevel * zoomLevelFactor);
}

ZoomFactor& ZoomFactor::operator/=(const float zoomLevelDivisor) {
	return operator=(zoomLevel / zoomLevelDivisor);
}

void ZoomFactor::addListener(Listener* const listener) {
	listeners.add(listener);
}

void ZoomFactor::removeListener(Listener* const listener) {
	listeners.remove(listener);
}

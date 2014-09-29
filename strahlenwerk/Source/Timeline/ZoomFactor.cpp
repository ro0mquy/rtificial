#include "ZoomFactor.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <Timeline/Timeline.h>
#include <cmath>

static const float minZoomLevel = 5.;

ZoomFactor::ZoomFactor() :
	zoomLevel(20.)
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
	zoomLevel = jmax(newZoomLevel, minZoomLevel); // without max() there will be dragons
	const float tmpZoomLevel = zoomLevel;
	zoomMutex.unlock();
	if (tmpZoomLevel != oldZoomLevel) {
		sendChangeMessage();
	}
	return *this;
}

ZoomFactor& ZoomFactor::operator*=(const float zoomLevelFactor) {
	zoomMutex.lock();
	const float oldZoomLevel = zoomLevel;
	zoomLevel *= zoomLevelFactor;
	zoomLevel = jmax(zoomLevel, minZoomLevel); // without max() there will be dragons
	const float tmpZoomLevel = zoomLevel;
	zoomMutex.unlock();
	if (tmpZoomLevel != oldZoomLevel) {
		sendChangeMessage();
	}
	return *this;
}

ZoomFactor& ZoomFactor::operator/=(const float zoomLevelDivisor) {
	zoomMutex.lock();
	const float oldZoomLevel = zoomLevel;
	zoomLevel /= zoomLevelDivisor;
	zoomLevel = jmax(zoomLevel, minZoomLevel); // without max() there will be dragons
	const float tmpZoomLevel = zoomLevel;
	zoomMutex.unlock();
	if (tmpZoomLevel != oldZoomLevel) {
		sendChangeMessage();
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

float ZoomFactor::getGridWith() {
	std::lock_guard<std::mutex> lock(zoomMutex);
	return std::pow(2., -std::floor(std::log2(zoomLevel / 20.)));
}

float ZoomFactor::snapValueToGrid(const float valueAsTime) {
	// snaps values that are in time units
	const float gridWidth = getGridWith();
	const float posOnGrid = valueAsTime / gridWidth;
	const float newRoundedPos = roundFloatToInt(posOnGrid) * gridWidth;
	return newRoundedPos;
}

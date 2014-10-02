#include "ZoomFactor.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <Timeline/Timeline.h>
#include <cmath>

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
	zoomLevel = newZoomLevel;
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

float ZoomFactor::getGridWidth() {
	std::lock_guard<std::mutex> lock(zoomMutex);
	return std::pow(2., -std::floor(std::log2(zoomLevel / 20.)));
}

float ZoomFactor::snapValueToGrid(const float valueAsTime) {
	// snaps values that are in time units
	const float gridWidth = getGridWidth();
	const float posOnGrid = valueAsTime / gridWidth;
	const float newRoundedPos = roundFloatToInt(posOnGrid) * gridWidth;
	return newRoundedPos;
}

float ZoomFactor::getEpsilon() {
	return getGridWidth() / 100.;
}

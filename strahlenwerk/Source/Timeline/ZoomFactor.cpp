#include "ZoomFactor.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <Timeline/Timeline.h>
#include <cmath>

static const float initialZoomLevel = .02f;
static const float minZoomLevel = .00015f;
static const float maxZoomLevel = 2.f;

ZoomFactor::ZoomFactor() :
	zoomLevel(initialZoomLevel)
{
}

ZoomFactor& ZoomFactor::getZoomFactor() {
	return StrahlenwerkApplication::getInstance()->getMainWindow().getMainContentComponent().getTimeline().getZoomFactor();
}

// multiply with ZoomFactor to convert time into pixels
// divide with ZoomFactor to convert pixels into time
ZoomFactor::operator float() {
	return zoomLevel;
}

ZoomFactor& ZoomFactor::operator=(const float newZoomLevel) {
	const float oldZoomLevel = zoomLevel;
	zoomLevel = jlimit(minZoomLevel, maxZoomLevel, newZoomLevel);
	const float tmpZoomLevel = zoomLevel;
	if (tmpZoomLevel != oldZoomLevel) {
		sendSynchronousChangeMessage();
	}
	return *this;
}

ZoomFactor& ZoomFactor::operator*=(const float zoomLevelFactor) {
	const float oldZoomLevel = zoomLevel;
	zoomLevel = jlimit(minZoomLevel, maxZoomLevel, zoomLevel * zoomLevelFactor);
	const float tmpZoomLevel = zoomLevel;
	if (tmpZoomLevel != oldZoomLevel) {
		sendSynchronousChangeMessage();
	}
	return *this;
}

ZoomFactor& ZoomFactor::operator/=(const float zoomLevelDivisor) {
	const float oldZoomLevel = zoomLevel;
	zoomLevel = jlimit(minZoomLevel, maxZoomLevel, zoomLevel / zoomLevelDivisor);
	const float tmpZoomLevel = zoomLevel;
	if (tmpZoomLevel != oldZoomLevel) {
		sendSynchronousChangeMessage();
	}
	return *this;
}

float ZoomFactor::timeToPixels(const int time) {
	return time * zoomLevel;
}

int ZoomFactor::pixelsToTime(const float pixels) {
	return roundFloatToInt(pixels / zoomLevel);
}

float ZoomFactor::getGridWidth() {
	return 250. * std::pow(2., -std::floor(std::log2(zoomLevel / initialZoomLevel)));
}

int ZoomFactor::snapValueToGrid(const int valueAsTime) {
	// snaps values that are in time units
	const float gridWidth = getGridWidth();
	const float posOnGrid = valueAsTime / gridWidth;
	const int newRoundedPos = roundFloatToInt(posOnGrid) * gridWidth;
	return newRoundedPos;
}

float ZoomFactor::getClippedFactor(const float originalZoomLevelFactor) {
	return jlimit(minZoomLevel / zoomLevel, maxZoomLevel / zoomLevel, originalZoomLevelFactor);
}

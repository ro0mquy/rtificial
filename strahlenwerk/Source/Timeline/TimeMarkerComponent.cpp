#include "TimeMarkerComponent.h"
#include <AudioManager.h>
#include "ZoomFactor.h"

TimeMarkerComponent::TimeMarkerComponent(ZoomFactor& zoomFactor_) :
	audioManager(AudioManager::getAudioManager()),
	zoomFactor(zoomFactor_)
{
	audioManager.addChangeListener(this);
	zoomFactor.addChangeListener(this);
	setOpaque(true);
}

TimeMarkerComponent::~TimeMarkerComponent() {
	audioManager.removeChangeListener(this);
	zoomFactor.removeChangeListener(this);
}

void TimeMarkerComponent::updatePosition() {
	// draw time marker
	const float newCenter = audioManager.getTimeInBeats() * zoomFactor;
	const int newX = roundFloatToInt(newCenter - getWidth() / 2.);
	setTopLeftPosition(newX, getY());
}

void TimeMarkerComponent::paint(Graphics& g) {
	g.fillAll(findColour(TimeMarkerComponent::fillColourId));
}

void TimeMarkerComponent::parentHierarchyChanged() {
	const int timeMarkerWidth = 2;
	setSize(timeMarkerWidth, getParentHeight());
	updatePosition();
}

void TimeMarkerComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor or time changed
	updatePosition();
}

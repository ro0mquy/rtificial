#include "TimeMarkerComponent.h"

#include <AudioManager.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/TimelineData.h>

TimeMarkerComponent::TimeMarkerComponent() :
	audioManager(AudioManager::getAudioManager())
{
	audioManager.addChangeListener(this);
	setInterceptsMouseClicks(false, false);
	setOpaque(true);
	setAlwaysOnTop(true);
}

TimeMarkerComponent::~TimeMarkerComponent() {
	audioManager.removeChangeListener(this);
}

void TimeMarkerComponent::updatePosition() {
	const float newCenter = getCurrentPosition();
	const int newX = roundFloatToInt(newCenter - getWidth() / 2.);
	setTopLeftPosition(newX, getY());
}

void TimeMarkerComponent::updateSize() {
	const int timeMarkerWidth = 2;
	setSize(timeMarkerWidth, getParentHeight());
}

void TimeMarkerComponent::paint(Graphics& g) {
	g.fillAll(findColour(TimeMarkerComponent::fillColourId));
}

void TimeMarkerComponent::parentHierarchyChanged() {
	if (getParentComponent() == nullptr) {
		return;
	}
	updateSize();
	updatePosition();
}

void TimeMarkerComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor or time changed
	updatePosition();
}


TimelineTimeMarkerComponent::TimelineTimeMarkerComponent(ZoomFactor& zoomFactor_) :
	zoomFactor(zoomFactor_)
{
	zoomFactor.addChangeListener(this);
}

TimelineTimeMarkerComponent::~TimelineTimeMarkerComponent() {
	zoomFactor.removeChangeListener(this);
}

float TimelineTimeMarkerComponent::getCurrentPosition() {
	return audioManager.getTime() * zoomFactor;
}


InspectorSequenceTimeMarkerComponent::InspectorSequenceTimeMarkerComponent(ValueTree sequenceData_) :
	sequenceData(sequenceData_),
	data(TimelineData::getTimelineData())
{
}

float InspectorSequenceTimeMarkerComponent::getCurrentPosition() {
	const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
	const float sequenceDuration = data.getSequenceDuration(sequenceData);
	return float(audioManager.getTime() - sequenceStart) / sequenceDuration * getParentWidth();
}


InspectorSceneTimeMarkerComponent::InspectorSceneTimeMarkerComponent(ValueTree sceneData_) :
	sceneData(sceneData_),
	data(TimelineData::getTimelineData())
{
}

float InspectorSceneTimeMarkerComponent::getCurrentPosition() {
	const int sceneStart = data.getSceneStart(sceneData);
	const float sceneDuration = data.getSceneDuration(sceneData);
	return float(audioManager.getTime() - sceneStart) / sceneDuration * getParentWidth();
}

#include "InspectorSequenceComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/ZoomFactor.h>
#include <Timeline/ScenesBarComponent.h>
#include <Timeline/SequenceComponent.h>
#include <Timeline/TimeMarkerComponent.h>
#include <AudioManager.h>

InspectorSequenceComponent::InspectorSequenceComponent() :
	InspectorSequenceComponent(ValueTree())
{
}

InspectorSequenceComponent::InspectorSequenceComponent(ValueTree sequenceData_) :
	data(TimelineData::getTimelineData()),
	audioManager(AudioManager::getAudioManager())
{
	setSequenceData(sequenceData_);
}

void InspectorSequenceComponent::paint(Graphics& g) {
	const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
	const int sequenceDuration = data.getSequenceDuration(sequenceData);
	const int sequenceEnd = sequenceStart + sequenceDuration;

	const float scaleFactor = float(getWidth()) / float(sequenceDuration);

	// draw ticks
	const int gridWidth = ZoomFactor::getZoomFactor().getGridWidth();
	const int firstLine = sequenceStart + ((gridWidth - (sequenceStart % gridWidth)) % gridWidth); // round up if not already on the grid

	const int longLineDistance = 4; // every nth tick is a long line
	const float longLineHeight = 30. / 2.;
	const float lineHeight = longLineHeight / 2.;

	for (int i = firstLine; i <= sequenceEnd; i += gridWidth) {
		const bool longLine = ((i / gridWidth) % longLineDistance == 0);
		g.setColour(findColour(ScenesBarComponent::tickColourId));
		g.drawLine(
				(i - sequenceStart) * scaleFactor + 0.5,
				0,
				(i - sequenceStart) * scaleFactor + 0.5,
				longLine ? longLineHeight : lineHeight,
				1
			);

		if (longLine) {
			g.setColour(findColour(ScenesBarComponent::textColourId));
			g.drawSingleLineText(String(i / 1000.), (i - sequenceStart) * scaleFactor + 1, .8 * g.getCurrentFont().getHeight());
		}
	}

	// draw sequence
	const float cornerSize = 5.0;
	Rectangle<float> boundsSequence = getLocalBounds().toFloat().withTrimmedTop(longLineHeight);

	g.setColour(findColour(SequenceComponent::fillColourId));
	g.fillRoundedRectangle(boundsSequence, cornerSize);

	g.setColour(findColour(SequenceComponent::outlineColourId));
	g.drawRoundedRectangle(boundsSequence, cornerSize, 1);
}

void InspectorSequenceComponent::paintOverChildren(Graphics& g) {
	// draw time marker
	const int timeMarkerWidth = 2;

	const int sequenceStart = data.getAbsoluteStartForSequence(sequenceData);
	const int sequenceDuration = data.getSequenceDuration(sequenceData);
	const int absoluteTime = audioManager.getTime();
	const int relativeTime = absoluteTime - sequenceStart;

	const float scaleFactor = float(getWidth()) / float(sequenceDuration);

	Rectangle<float> timeMarkerRect(0, 0, timeMarkerWidth, getHeight());
	timeMarkerRect.setCentre(relativeTime * scaleFactor, timeMarkerRect.getCentreY());
	g.setColour(findColour(TimeMarkerComponent::fillColourId));
	g.fillRect(timeMarkerRect);
}

void InspectorSequenceComponent::setSequenceData(ValueTree sequenceData_) {
	sequenceData = sequenceData_;
	addAllKeyframeComponents();
	repaint();
}

void InspectorSequenceComponent::addKeyframeComponent(ValueTree keyframeData) {
	ZoomFactor& zoomFactor = ZoomFactor::getZoomFactor();
	auto keyframeComponent = new InspectorKeyframeComponent(*this, keyframeData, zoomFactor);
	addAndMakeVisible(keyframeComponent);
	keyframeComponentsArray.add(keyframeComponent);
}

void InspectorSequenceComponent::addAllKeyframeComponents() {
	keyframeComponentsArray.clearQuick(true);
	const int numKeyframes = data.getNumKeyframes(sequenceData);

	// don't create a component for first and last keyframe
	for (int i = 1; i < numKeyframes - 1; i++) {
		ValueTree keyframeData = data.getKeyframe(sequenceData, i);
		addKeyframeComponent(keyframeData);
	}
}

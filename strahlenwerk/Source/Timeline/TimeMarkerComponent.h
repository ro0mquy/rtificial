#ifndef TIMEMARKERCOMPONENT_H
#define TIMEMARKERCOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"

class AudioManager;
class ZoomFactor;
class TimelineData;

class TimeMarkerComponent :
	public McbComponent,
	protected ChangeListener
{
	public:
		TimeMarkerComponent();
		~TimeMarkerComponent();

		virtual float getCurrentPosition() = 0;
		void updatePosition();
		void updateSize();

		void paint(Graphics& g) override;
		void parentHierarchyChanged() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		enum ColourIds{
			fillColourId = 0x9295300,
		};

	protected:
		AudioManager& audioManager;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeMarkerComponent)
};

class TimelineTimeMarkerComponent :
	public TimeMarkerComponent
{
	public:
		TimelineTimeMarkerComponent(ZoomFactor& zoomFactor_);
		~TimelineTimeMarkerComponent();

		float getCurrentPosition() override;

	private:
		ZoomFactor& zoomFactor;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineTimeMarkerComponent)
};

class InspectorTimeMarkerComponent :
	public TimeMarkerComponent
{
	public:
		InspectorTimeMarkerComponent(ValueTree sequenceData_);

		float getCurrentPosition() override;

	private:
		ValueTree sequenceData;
		TimelineData& data;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorTimeMarkerComponent)
};

#endif // TIMEMARKERCOMPONENT_H

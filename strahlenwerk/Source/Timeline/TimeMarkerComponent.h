#ifndef TIMEMARKERCOMPONENT_H
#define TIMEMARKERCOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"

class AudioManager;
class ZoomFactor;

class TimeMarkerComponent :
	public McbComponent,
	private ChangeListener
{
	public:
		TimeMarkerComponent(ZoomFactor& zoomFactor_);
		~TimeMarkerComponent();

		void updatePosition();

		void paint(Graphics& g) override;
		void parentHierarchyChanged() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		enum ColourIds{
			fillColourId = 0x9295300,
		};

	private:
		AudioManager& audioManager;
		ZoomFactor& zoomFactor;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeMarkerComponent)
};

#endif // TIMEMARKERCOMPONENT_H

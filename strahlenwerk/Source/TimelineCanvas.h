#ifndef TIMELINECANVAS_H
#define TIMELINECANVAS_H

#include "../JuceLibraryCode/JuceHeader.h"

class TimelineCanvas : public Component
{
	public:
		TimelineCanvas();
		void resized() override;

	private:
		TextButton button;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineCanvas)
};

#endif // TIMELINECANVAS_H

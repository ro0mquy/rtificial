#ifndef TIMELINE_H
#define TIMELINE_H

#include "../JuceLibraryCode/JuceHeader.h"

// extend Viewport with a custom callback function
class ViewportCallback : public Viewport {
	public:
		void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override;
};

class TimelineCanvas : public Component
{
	public:
		TimelineCanvas();
		void resized() override;

	private:
		TextButton button;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineCanvas)
};

class Timeline : public Component
{
	public:
		Timeline();
		void resized() override;
		void callbackViewportChanged(ViewportCallback* vp, Point<int> position);

	private:
		ViewportCallback viewportCanvas;
		ViewportCallback viewportScenes;
		ViewportCallback viewportUniforms;
		TimelineCanvas componentCanvas;
		TimelineCanvas componentScenes;
		TimelineCanvas componentUniforms;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};

#endif // TIMELINE_H

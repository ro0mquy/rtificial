#ifndef TIMELINE_H
#define TIMELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "TimelineCanvas.h"

class TimelineScenes : public Component {
	public:
		TimelineScenes(Value& timeValue);
		void resized() override;
		void paint(Graphics& g) override;

	private:
		Value currentTime;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineScenes)
};

class TimelineUniforms : public Component {
	public:
		TimelineUniforms(Value& timeValue);
		void paint(Graphics& g) override;

	private:
		Value currentTime;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineUniforms)
};

class Timeline : public Component
{
	public:
		Timeline();
		void resized() override;

	private:
		// extend Viewport with a custom callback function
		class ViewportCallback : public Viewport {
			public:
				ViewportCallback(Timeline& timelineParent);
				void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override;

			private:
				Timeline& timelineParent;
		};

		void callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position);

		ViewportCallback viewportCanvas;
		ViewportCallback viewportScenes;
		ViewportCallback viewportUniforms;
		TimelineCanvas componentCanvas;
		TimelineScenes componentScenes;
		TimelineUniforms componentUniforms;

		Value currentTime;
		ValueTree valueTree;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};

#endif // TIMELINE_H

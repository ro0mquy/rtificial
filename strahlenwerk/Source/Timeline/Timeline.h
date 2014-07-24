#ifndef TIMELINE_H
#define TIMELINE_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SequenceViewComponent.h"

class ScenesBarComponent : public Component {
	public:
		ScenesBarComponent(Value& timeValue);
		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102
		};

	private:
		Value currentTime;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

class UniformsBarComponent : public Component {
	public:
		UniformsBarComponent(Value& timeValue);
		void paint(Graphics& g) override;

	private:
		Value currentTime;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UniformsBarComponent)
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
		void setupValueTree();

		Value currentTime;
		ValueTree valueTree;

		ViewportCallback viewportCanvas;
		ViewportCallback viewportScenes;
		ViewportCallback viewportUniforms;
		SequenceViewComponent sequenceView;
		ScenesBarComponent scenesBar;
		UniformsBarComponent uniformsBar;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};

#endif // TIMELINE_H

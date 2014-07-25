#ifndef TIMELINE_H
#define TIMELINE_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SequenceViewComponent.h"
#include "Data.h"

class ScenesBarComponent : public Component {
	public:
		ScenesBarComponent(Value& timeValue, Data& _data);
		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102,
			sceneColourId = 0x2300103,
			sceneOutlineColourId = 0x2300104,
			sceneTextColourId = 0x2300105
		};

	private:
		Value currentTime;
		Data& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

class UniformsBarComponent : public Component {
	public:
		UniformsBarComponent(Value& timeValue, Data& _data);
		void paint(Graphics& g) override;

	private:
		Value currentTime;
		Data& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UniformsBarComponent)
};

class Timeline : public Component
{
	public:
		Timeline();
		void resized() override;

		// extend Viewport with a custom callback function
		class ViewportCallback : public Viewport {
			public:
				ViewportCallback(Timeline& timelineParent) :
					timelineParent(timelineParent) {}

				// make a callback to Timeline if one of the viewports changed
				void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override {
					timelineParent.callbackViewportChanged(this, newVisibleArea.getPosition());
				}


			private:
				Timeline& timelineParent;
		};

	private:
		void callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position);
		void setupValueTree();

		Value currentTime;
		Data data;

		ViewportCallback viewportSequenceView;
		ViewportCallback viewportScenesBar;
		ViewportCallback viewportUniformsBar;
		SequenceViewComponent sequenceView;
		ScenesBarComponent scenesBar;
		UniformsBarComponent uniformsBar;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};

#endif // TIMELINE_H

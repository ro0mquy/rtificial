#ifndef TIMELINE_H
#define TIMELINE_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SequenceViewComponent.h"
#include "ScenesBar.h"
#include "Data.h"

class UniformsBarComponent : public Component {
	public:
		UniformsBarComponent(Value& timeValue, Data& _data);
		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds{
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			uniformTextColourId = 0x2300303,
			seperatorColourId = 0x2300304
		};

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

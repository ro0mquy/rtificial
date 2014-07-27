#ifndef TIMELINE_H
#define TIMELINE_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SequenceViewComponent.h"
#include "ScenesBarComponent.h"
#include "UniformsBarComponent.h"
#include "Data.h"

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

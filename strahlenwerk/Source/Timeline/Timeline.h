#ifndef TIMELINE_H
#define TIMELINE_H

#include <juce>
#include "SequenceViewComponent.h"
#include "ScenesBarComponent.h"
#include "UniformsBarComponent.h"
#include "ZoomFactor.h"
#include "SnapToGridConstrainer.h"

class Timeline : public Component
{
	public:
		Timeline();
		void resized() override;
		void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;

		ZoomFactor& getZoomFactor();

		// extend Viewport with a custom callback function
		class ViewportCallback : public Viewport {
				// make a callback to Timeline if one of the viewports changed
				void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override {
					Timeline* timelineParent = findParentComponentOfClass<Timeline>();
					timelineParent->callbackViewportChanged(this, newVisibleArea.getPosition());
				}
		};

	private:
		void callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position);

		ZoomFactor zoomFactor;
		SnapToGridConstrainer constrainer;

		ViewportCallback viewportSequenceView;
		ViewportCallback viewportScenesBar;
		ViewportCallback viewportUniformsBar;
		Component sequenceView;
		ScenesBarComponent scenesBar;
		UniformsBarComponent uniformsBar;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};

#endif // TIMELINE_H

#ifndef TIMELINE_H
#define TIMELINE_H

#include <juce>
#include <BoxLayout.h>
#include "SelfResizingComponent.h"
#include "SequenceViewComponent.h"
#include "ScenesBarComponent.h"
#include "UniformsBarComponent.h"
#include "ZoomFactor.h"

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
			private:
				// make a callback to Timeline if one of the viewports changed
				void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override {
					Timeline* timelineParent = findParentComponentOfClass<Timeline>();

					if (timelineParent != nullptr) {
						timelineParent->callbackViewportChanged(this, newVisibleArea.getPosition());
					}

				}

			public:
				// update viewed components size on resize
				void resized() override {
					Viewport::resized();

					SelfResizingComponent* comp = dynamic_cast<SelfResizingComponent*>(getViewedComponent());
					if (comp != nullptr) {
						comp->updateSize();
					}
				}
		};

	private:
		void callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position);

		ZoomFactor zoomFactor;

		ViewportCallback viewportSequenceView;
		ViewportCallback viewportScenesBar;
		ViewportCallback viewportUniformsBar;
		SequenceViewComponent sequenceView;
		ScenesBarComponent scenesBar;
		UniformsBarComponent uniformsBar;

		StretchableLayoutManager scenesBarSequenceViewLayout;
		BoxLayout<2, true> scenesBarSequenceViewBoxLayout;

		StretchableLayoutResizerBar uniformsBarResizer;
		StretchableLayoutManager timelineLayout;
		BoxLayout<3, false, false> timelineBoxLayout;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};

#endif // TIMELINE_H

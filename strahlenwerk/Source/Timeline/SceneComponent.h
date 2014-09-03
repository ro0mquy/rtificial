#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include <juce>
#include "SnapToGridConstrainer.h"
#include "ZoomFactor.h"

class TimelineData;

class SceneComponent :
	public Component,
	private ComponentDragger,
	private ZoomFactor::Listener
{
	public:
		SceneComponent(ValueTree _sceneData, ZoomFactor& zoomFactor_);
		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void moved() override;
		void resized() override;
		void parentHierarchyChanged() override;
		void zoomFactorChanged(ZoomFactor&) override;

		void updateBounds();

		enum ColourIds {
			fillColourId = 0x2300103,
			outlineColourId = 0x2300104,
			textColourId = 0x2300105,
		};

	private:
		ValueTree sceneData;
		TimelineData& data;
		ZoomFactor& zoomFactor;

		SnapToGridConstrainer constrainer;
		ResizableBorderComponent resizableBorder;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)
};

#endif

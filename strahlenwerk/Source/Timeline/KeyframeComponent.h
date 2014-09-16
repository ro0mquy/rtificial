#ifndef KEYFRAMECOMPONENT_H
#define KEYFRAMECOMPONENT_H

#include <juce>
#include "SnapToGridConstrainer.h"
#include "ZoomFactor.h"
#include "MouseCallbackClasses.h"

class TimelineData;

class KeyframeComponent :
	public McbComponent,
	private ComponentDragger,
	private ValueTree::Listener,
	private ZoomFactor::Listener
{
	public:
		KeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_);

		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void moved() override;
		void parentHierarchyChanged() override;
		void zoomFactorChanged(ZoomFactor&) override;

		void updateBounds();

        void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
        void valueTreeChildOrderChanged(ValueTree& parentTree) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		enum ColourIds {
			fillColourId = 0x9414001,
		};

	private:
		ValueTree keyframeData;
		TimelineData& data;
		ZoomFactor& zoomFactor;
		SnapToGridConstrainer constrainer;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyframeComponent)
};

#endif // KEYFRAMECOMPONENT_H

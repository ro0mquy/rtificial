#ifndef KEYFRAMECOMPONENT_H
#define KEYFRAMECOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"

class TimelineData;
class ZoomFactor;

class KeyframeComponent :
	public McbComponent,
	private ComponentDragger,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		KeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_);
		~KeyframeComponent();

		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void parentHierarchyChanged() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		void updateBounds();

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		enum ColourIds {
			fillColourId = 0x9414001,
		};

		ValueTree keyframeData;

	private:
		TimelineData& data;
		ZoomFactor& zoomFactor;
		ComponentBoundsConstrainer constrainer;

		class Positioner : public Component::Positioner {
			public:
				Positioner(Component& component, ValueTree keyframeData_, TimelineData& data_, ZoomFactor& zoomFactor_);
				void applyNewBounds(const Rectangle<int>& newBounds) override;

			private:
				ValueTree keyframeData;
				TimelineData& data;
				ZoomFactor& zoomFactor;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Positioner)
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyframeComponent)
};

#endif // KEYFRAMECOMPONENT_H

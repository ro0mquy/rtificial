#ifndef SEQUENCECOMPONENT_H
#define SEQUENCECOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"

class TimelineData;
class ZoomFactor;
class KeyframeComponent;

class SequenceComponent :
	public McbComponent,
	private ComponentDragger,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		SequenceComponent(ValueTree _sequenceData, ZoomFactor& zoomFactor_);
		~SequenceComponent();

		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void resized() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		void updateBounds();
		void addKeyframeComponent(ValueTree keyframeData);
		void addAllKeyframeComponents();
		KeyframeComponent* getKeyframeComponentForData(ValueTree keyframeData);

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		const String getInterpolationMethod();

		enum ColourIds {
			fillColourId = 0x4934001,
			highlightedFillColourId,
			outlineColourId,
			textColourId,
			highlightedTextColourId,
		};

		ValueTree sequenceData;

	private:
		TimelineData& data;
		ZoomFactor& zoomFactor;

		ComponentBoundsConstrainer constrainer;
		ResizableBorderComponent resizableBorder;
		OwnedArray<KeyframeComponent> keyframeComponentsArray;

		class Positioner : public Component::Positioner {
			public:
				Positioner(Component& component, ValueTree sequenceData_, TimelineData& data_, ZoomFactor& zoomFactor_);
				void applyNewBounds(const Rectangle<int>& newBounds) override;

			private:
				ValueTree sequenceData;
				TimelineData& data;
				ZoomFactor& zoomFactor;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Positioner)
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceComponent)
};

#endif // SEQUENCECOMPONENT_H

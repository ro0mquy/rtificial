#ifndef INSPECTORSEQUENCECOMPONENT_H
#define INSPECTORSEQUENCECOMPONENT_H

#include <juce>

#include <Timeline/MouseCallbackClasses.h>
#include <Timeline/KeyframeComponent.h>

class TimelineData;
class ZoomFactor;

class InspectorSequenceComponent :
	public McbComponent,
	private ValueTree::Listener
{
	public:
		InspectorSequenceComponent(ValueTree sequenceData_);
		~InspectorSequenceComponent();

		void resized() override;
		void paint(Graphics& g) override;

		void addKeyframeComponent(ValueTree keyframeData);
		void addAllKeyframeComponents();
		KeyframeComponent* getKeyframeComponentForData(ValueTree keyframeData);

		void mouseUp(const MouseEvent& event) override;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		ValueTree sequenceData;

	private:
		TimelineData& data;
		ZoomFactor& zoomFactor;
		OwnedArray<KeyframeComponent> keyframeComponentsArray;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorSequenceComponent)
};

#endif // INSPECTORSEQUENCECOMPONENT_H

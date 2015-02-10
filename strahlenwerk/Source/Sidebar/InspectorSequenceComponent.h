#ifndef INSPECTORSEQUENCECOMPONENT_H
#define INSPECTORSEQUENCECOMPONENT_H

#include <juce>

#include <Timeline/KeyframeComponent.h>
#include <Timeline/TimeMarkerComponent.h>

class TimelineData;
class ZoomFactor;
class AudioManager;

class InspectorSequenceComponent :
	public Component,
	private ValueTree::Listener
{
	public:
		InspectorSequenceComponent(ValueTree sequenceData_);
		~InspectorSequenceComponent();

		void paint(Graphics& g) override;

		void addKeyframeComponent(ValueTree keyframeData);
		void addAllKeyframeComponents();
		KeyframeComponent* getKeyframeComponentForData(ValueTree keyframeData);

		void mouseUp(const MouseEvent& event);

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		ValueTree sequenceData;

	private:
		TimelineData& data;
		AudioManager& audioManager;
		ZoomFactor& zoomFactor;
		InspectorTimeMarkerComponent timeMarker;
		OwnedArray<KeyframeComponent> keyframeComponentsArray;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorSequenceComponent)
};

#endif // INSPECTORSEQUENCECOMPONENT_H

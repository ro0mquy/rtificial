#ifndef SEQUENCEBACKGROUNDCOMPONENT_H
#define SEQUENCEBACKGROUNDCOMPONENT_H

#include <juce>

#include <Sidebar/InspectorSequenceComponent.h>
#include <Timeline/TimeMarkerComponent.h>

class TimelineData;
class ZoomFactor;

class SequenceBackgroundComponent :
	public Component,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		SequenceBackgroundComponent(ValueTree sequenceData_);
		~SequenceBackgroundComponent();

		void resized() override;
		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;

		void changeListenerCallback(ChangeBroadcaster* source);

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

	private:
		ValueTree sequenceData;
		InspectorSequenceComponent sequenceComponent;
		InspectorTimeMarkerComponent timeMarker;
		TimelineData& data;
		ZoomFactor& zoomFactor;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceBackgroundComponent)
};

#endif // SEQUENCEBACKGROUNDCOMPONENT_H

#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include <juce>
#include "ZoomFactor.h"
#include "MouseCallbackClasses.h"

class TimelineData;
class SequenceComponent;

class SequenceViewComponent :
	public McbComponent,
	private ValueTree::Listener,
	private ZoomFactor::Listener,
	private Value::Listener
{
	public:
		SequenceViewComponent(ZoomFactor& zoomFactor_);
		~SequenceViewComponent();

		void updateSize();
		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;
		void addSequenceComponent(ValueTree sequenceData);
		void addAllSequenceComponents();
		SequenceComponent* getSequenceComponentForData(ValueTree sequenceData);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		void zoomFactorChanged(ZoomFactor&) override;
		void valueChanged(Value& value) override;

        void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
        void valueTreeChildOrderChanged(ValueTree& parentTree) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		enum ColourIds{
			timeMarkerColourId = 0x8350102,
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			seperatorColourId = 0x2300304,
			inactiveAreaColourId = 0x2300305,
		};

	private:
		bool uniformActiveForScene(ValueTree uniform, ValueTree scene);

		TimelineData& data;
		ZoomFactor& zoomFactor;
		OwnedArray<SequenceComponent> sequenceComponentsArray;

		ValueTree newSequenceData;
		ScopedPointer<SequenceComponent> newSequenceComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceViewComponent)
};

#endif // SEQUENCEVIEWCOMPONENT_H

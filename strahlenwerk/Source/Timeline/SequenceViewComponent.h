#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include <juce>
#include "ZoomFactor.h"

class TimelineData;
class SequenceComponent;

class SequenceViewComponent :
	public Component,
	private ZoomFactor::Listener
{
	public:
		SequenceViewComponent(ZoomFactor& zoomFactor_);
		~SequenceViewComponent();

		void updateSize();
		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;
		void updateSequenceComponents();
		void removeSequenceComponent(const SequenceComponent* toBeDeleted);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		void zoomFactorChanged(ZoomFactor&) override;

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

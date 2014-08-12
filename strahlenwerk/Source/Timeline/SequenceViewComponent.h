#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include <juce>
#include "Data.h"
#include "SequenceComponent.h"

class SequenceViewComponent : public Component
{
	public:
		SequenceViewComponent(Value& timeValue, Data& _data);
		void updateSize();
		void paint(Graphics& g) override;
		void updateSequenceComponents();

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		enum ColourIds{
			timeMarkerColourId = 0x8350102,
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			seperatorColourId = 0x2300304,
			inactiveAreaColourId = 0x2300305,
		};

	private:
		bool uniformActiveForScene(ValueTree uniform, ValueTree scene);

		Value currentTime;
		Data& data;
		OwnedArray<SequenceComponent> sequenceComponentsArray;

		ValueTree newSequenceData;
		SequenceComponent* newSequenceComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceViewComponent)
};

#endif // SEQUENCEVIEWCOMPONENT_H

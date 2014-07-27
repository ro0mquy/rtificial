#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"

class SequenceViewComponent : public Component
{
	public:
		SequenceViewComponent(Value& timeValue, Data& _data);
		void updateSize();
		void paint(Graphics& g) override;

		enum ColourIds{
			timeMarkerColourId = 0x8350102,
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			seperatorColourId = 0x2300304,
			unactiveAreaColourId = 0x2300305,
		};

	private:
		bool uniformActiveForScene(ValueTree uniform, ValueTree scene);

		Value currentTime;
		Data& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceViewComponent)
};

#endif // SEQUENCEVIEWCOMPONENT_H

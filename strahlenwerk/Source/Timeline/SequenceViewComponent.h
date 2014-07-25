#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"

class SequenceViewComponent : public Component
{
	public:
		SequenceViewComponent(Value& timeValue, Data& _data);
		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds{
			timeMarkerColourId = 0x8350102,
		};

	private:
		TextButton button;
		Value currentTime;
		Data& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceViewComponent)
};

#endif // SEQUENCEVIEWCOMPONENT_H

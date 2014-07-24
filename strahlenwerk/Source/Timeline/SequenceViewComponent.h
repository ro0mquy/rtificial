#ifndef SEQUENCEVIEWCOMPONENT_H
#define SEQUENCEVIEWCOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"

class SequenceViewComponent : public Component
{
	public:
		SequenceViewComponent(Value& timeValue);
		void resized() override;

	private:
		TextButton button;
		Value currentTime;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceViewComponent)
};

#endif // SEQUENCEVIEWCOMPONENT_H

#ifndef SEQUENCECOMPONENT_H
#define SEQUENCECOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"

class SequenceComponent :
	public Component,
	private Value::Listener
{
	public:
		SequenceComponent(ValueTree _sequenceData, Data& _data, int y, int height);
		void paint(Graphics& g) override;
		void valueChanged(Value& /*value*/) override;

		void updateBounds();
		int getAbsoluteStart();

		enum ColourIds {
			fillColourId = 0x4934001,
			outlineColourId = 0x4934002,
		};

	private:
		ValueTree sequenceData;
		Data& data;
		Value sceneStartValue;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceComponent)
};

#endif // SEQUENCECOMPONENT_H

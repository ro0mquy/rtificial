#ifndef UNIFORMSBARCOMPONENT_H
#define UNIFORMSBARCOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"

class UniformsBarComponent : public Component {
	public:
		UniformsBarComponent(Value& timeValue, Data& _data);
		void updateSize();
		void paint(Graphics& g) override;

		enum ColourIds{
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			uniformTextColourId = 0x2300303,
			seperatorColourId = 0x2300304
		};

	private:
		Value currentTime;
		Data& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UniformsBarComponent)
};

#endif // UNIFORMSBARCOMPONENT_H

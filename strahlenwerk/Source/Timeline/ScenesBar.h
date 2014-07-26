#ifndef SCENESBAR_H
#define SCENESBAR_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"

class ScenesBarComponent : public Component {
	public:
		ScenesBarComponent(Value& timeValue, Data& _data);
		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102,
			sceneColourId = 0x2300103,
			sceneOutlineColourId = 0x2300104,
			sceneTextColourId = 0x2300105

		};

	private:
		Value currentTime;
		Data& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

#endif // SCENESBAR_H

#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"

class SceneComponent : public Component, private Timer {
	public:
		SceneComponent(ValueTree _sceneData);
		void paint(Graphics& g) override;
		void timerCallback() override;

		enum ColourIds {
			fillColourId = 0x2300103,
			outlineColourId = 0x2300104,
			textColourId = 0x2300105,
		};

	private:
		ValueTree sceneData;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)
};

#endif

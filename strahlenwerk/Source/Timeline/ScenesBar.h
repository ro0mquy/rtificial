#ifndef SCENESBAR_H
#define SCENESBAR_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"
#include "SceneComponent.h"

class ScenesBarComponent : public Component {
	public:
		ScenesBarComponent(Value& timeValue, Data& _data);
		void resized() override;
		void paint(Graphics& g) override;
		void updateSceneComponents();

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102,
		};

	private:
		Value currentTime;
		Data& data;
		OwnedArray<SceneComponent> sceneComponentsArray;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

#endif // SCENESBAR_H

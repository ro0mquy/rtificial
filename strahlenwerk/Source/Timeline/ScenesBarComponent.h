#ifndef SCENESBARCOMPONENT_H
#define SCENESBARCOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Data.h"
#include "SceneComponent.h"

class ScenesBarComponent : public Component {
	public:
		ScenesBarComponent(Value& timeValue, Data& _data);
		void updateSize();
		void paint(Graphics& g) override;
		void updateSceneComponents();

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102,
		};

	private:
		Value currentTime;
		Data& data;
		OwnedArray<SceneComponent> sceneComponentsArray;

		ValueTree newSceneData;
		SceneComponent* newSceneComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

#endif // SCENESBARCOMPONENT_H

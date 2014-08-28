#ifndef SCENESBARCOMPONENT_H
#define SCENESBARCOMPONENT_H

#include <juce>

class Data;
class SceneComponent;

class ScenesBarComponent : public Component {
	public:
		ScenesBarComponent();
		~ScenesBarComponent();

		void updateSize();
		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;
		void updateSceneComponents();

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102,
		};

	private:
		Data& data;
		OwnedArray<SceneComponent> sceneComponentsArray;

		ValueTree newSceneData;
		SceneComponent* newSceneComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

#endif // SCENESBARCOMPONENT_H

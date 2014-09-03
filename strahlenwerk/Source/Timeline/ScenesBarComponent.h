#ifndef SCENESBARCOMPONENT_H
#define SCENESBARCOMPONENT_H

#include <juce>
#include "ZoomFactor.h"

class TimelineData;
class SceneComponent;

class ScenesBarComponent :
	public Component,
	private ZoomFactor::Listener
{
	public:
		ScenesBarComponent(ZoomFactor& zoomFactor_);
		~ScenesBarComponent();

		void updateSize();
		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;
		void updateSceneComponents();
		void removeSceneComponent(const SceneComponent* toBeDeleted);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		void zoomFactorChanged(ZoomFactor&) override;

		enum ColourIds{
			tickColourId = 0x2300101,
			timeMarkerColourId = 0x2300102,
		};

	private:
		TimelineData& data;
		ZoomFactor& zoomFactor;
		OwnedArray<SceneComponent> sceneComponentsArray;

		ValueTree newSceneData;
		ScopedPointer<SceneComponent> newSceneComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

#endif // SCENESBARCOMPONENT_H

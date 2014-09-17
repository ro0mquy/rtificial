#ifndef SCENESBARCOMPONENT_H
#define SCENESBARCOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"
#include "ZoomFactor.h"

class TimelineData;
class SceneComponent;

class ScenesBarComponent :
	public McbComponent,
	private ValueTree::Listener,
	private ZoomFactor::Listener,
	private Value::Listener
{
	public:
		ScenesBarComponent(ZoomFactor& zoomFactor_);
		~ScenesBarComponent();

		void updateSize();
		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;
		void addSceneComponent(ValueTree sceneData);
		void addAllSceneComponents();
		void removeSceneComponent(const SceneComponent* toBeDeleted);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		void zoomFactorChanged(ZoomFactor&) override;
		void valueChanged(Value& value) override;

        void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
        void valueTreeChildOrderChanged(ValueTree& parentTree) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

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

#ifndef SCENEBACKGROUNDCOMPONENT_H
#define SCENEBACKGROUNDCOMPONENT_H

#include <juce>

#include <Timeline/TimeMarkerComponent.h>

class TimelineData;
class ZoomFactor;

class SceneBackgroundComponent :
	public Component,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		SceneBackgroundComponent(ValueTree sceneData_);
		~SceneBackgroundComponent();

		void resized() override;
		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;

		void changeListenerCallback(ChangeBroadcaster* source) override;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

	private:
		ValueTree sceneData;
		InspectorSceneTimeMarkerComponent timeMarker;
		TimelineData& data;
		ZoomFactor& zoomFactor;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneBackgroundComponent)
};

#endif // SCENEBACKGROUNDCOMPONENT_H

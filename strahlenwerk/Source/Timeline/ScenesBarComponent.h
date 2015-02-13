#ifndef SCENESBARCOMPONENT_H
#define SCENESBARCOMPONENT_H

#include <juce>
#include "SelfResizingComponent.h"
#include "MouseCallbackClasses.h"
#include "TimeMarkerComponent.h"

class TimelineData;
class AudioManager;
class ZoomFactor;
class SceneComponent;

class ScenesBarComponent :
	public McbComponent,
	public SelfResizingComponent,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		ScenesBarComponent(ZoomFactor& zoomFactor_);
		~ScenesBarComponent();

		void updateSize() override;
		void paint(Graphics& g) override;
		void addSceneComponent(ValueTree sceneData);
		void addAllSceneComponents();
		SceneComponent* getSceneComponentForData(ValueTree sceneData);

		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;

		void changeListenerCallback(ChangeBroadcaster* source) override;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
		void valueTreeRedirected(ValueTree& treeWhoWasRedirected) override;

		enum ColourIds{
			tickColourId = 0x2300100,
			waveformColourId,
			textColourId,
			outlineColourId,
		};

	private:
		TimelineData& data;
		ZoomFactor& zoomFactor;
		TimeMarkerComponent timeMarker;
		OwnedArray<SceneComponent> sceneComponentsArray;

		ValueTree currentlyCreatedSceneData;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScenesBarComponent)
};

#endif // SCENESBARCOMPONENT_H

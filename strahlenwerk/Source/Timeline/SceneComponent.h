#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"

class TimelineData;
class ZoomFactor;

class SceneComponent :
	public McbComponent,
	private ComponentDragger,
	private ValueTree::Listener,
	private ChangeListener
{
	public:
		SceneComponent(ValueTree _sceneData, ZoomFactor& zoomFactor_);
		~SceneComponent();

		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void resized() override;
		void parentHierarchyChanged() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		void updateBounds();

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		enum ColourIds {
			fillColourId = 0x2300103,
			highlightedFillColourId,
			outlineColourId,
			textColourId,
			highlightedTextColourId,
		};

		ValueTree sceneData;

	private:
		TimelineData& data;
		ZoomFactor& zoomFactor;

		ComponentBoundsConstrainer constrainer;
		McbResizableBorder resizableBorder;

		ValueTree currentlyCopiedSceneData;

		class Positioner : public Component::Positioner {
			public:
				Positioner(Component& component, ValueTree sceneData_, TimelineData& data_, ZoomFactor& zoomFactor_);
				void applyNewBounds(const Rectangle<int>& newBounds) override;

			private:
				ValueTree sceneData;
				TimelineData& data;
				ZoomFactor& zoomFactor;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Positioner)
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)
};

#endif

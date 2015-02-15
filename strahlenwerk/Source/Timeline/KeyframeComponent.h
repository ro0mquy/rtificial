#ifndef KEYFRAMECOMPONENT_H
#define KEYFRAMECOMPONENT_H

#include <juce>
#include "MouseCallbackClasses.h"

class TimelineData;
class ZoomFactor;
class InspectorSequenceComponent;

class KeyframeComponent :
	public McbComponent,
	private ComponentDragger,
	protected ValueTree::Listener
{
	public:
		KeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_);
		~KeyframeComponent();

		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void parentHierarchyChanged() override;

		void updateBounds();

		virtual float timeToPixels(const int time) = 0;
		virtual int pixelsToTime(const float pixels) = 0;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		enum ColourIds {
			fillColourId = 0x9414001,
		};

		ValueTree keyframeData;

	protected:
		TimelineData& data;
		ZoomFactor& zoomFactor;

	private:
		ComponentBoundsConstrainer constrainer;

		class Positioner : public Component::Positioner {
			public:
				Positioner(KeyframeComponent& component, ValueTree keyframeData_, TimelineData& data_, ZoomFactor& zoomFactor_);
				void applyNewBounds(const juce::Rectangle<int>& newBounds) override;

			private:
				ValueTree keyframeData;
				KeyframeComponent& keyframeComponent;
				TimelineData& data;
				ZoomFactor& zoomFactor;

				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Positioner)
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyframeComponent)
};

class TimelineKeyframeComponent :
	public KeyframeComponent,
	private ChangeListener
{
	public:
		TimelineKeyframeComponent(ValueTree keyframeData_, ZoomFactor& zoomFactor_);
		~TimelineKeyframeComponent();

		float timeToPixels(const int time) override;
		int pixelsToTime(const float pixels) override;

		void changeListenerCallback(ChangeBroadcaster* source) override;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimelineKeyframeComponent)
};

class InspectorKeyframeComponent :
	public KeyframeComponent
{
	public:
		InspectorKeyframeComponent(InspectorSequenceComponent& sequenceComponent_, ValueTree keyframeData_, ZoomFactor& zoomFactor_);
		~InspectorKeyframeComponent();

		float timeToPixels(const int time) override;
		int pixelsToTime(const float pixels) override;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;

	private:
		InspectorSequenceComponent& sequenceComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorKeyframeComponent)
};

#endif // KEYFRAMECOMPONENT_H

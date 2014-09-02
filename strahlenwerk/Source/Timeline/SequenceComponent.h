#ifndef SEQUENCECOMPONENT_H
#define SEQUENCECOMPONENT_H

#include <juce>
#include "SnapToGridConstrainer.h"

class TimelineData;
class KeyframeComponent;

class SequenceComponent :
	public Component,
	private ComponentDragger,
	private Value::Listener
{
	public:
		SequenceComponent(ValueTree _sequenceData, int y, int height);
		void paint(Graphics& g) override;
		void valueChanged(Value& /*value*/) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void moved() override;
		void resized() override;

		void updateBounds();
		void updateSceneStartValueRefer();
		void updateKeyframeComponents();
		int getAbsoluteStart();
		void removeKeyframeComponent(const KeyframeComponent* toBeDeleted);

		enum ColourIds {
			fillColourId = 0x4934001,
			outlineColourId = 0x4934002,
		};

	private:
		ValueTree sequenceData;
		TimelineData& data;
		Value sceneStartValue;
		SnapToGridConstrainer constrainer;
		ResizableBorderComponent resizableBorder;
		OwnedArray<KeyframeComponent> keyframeComponentsArray;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceComponent)
};

#endif // SEQUENCECOMPONENT_H

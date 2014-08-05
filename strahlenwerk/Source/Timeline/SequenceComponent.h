#ifndef SEQUENCECOMPONENT_H
#define SEQUENCECOMPONENT_H

#include <juce>
#include "Data.h"
#include "SnapToGridConstrainer.h"

class SequenceComponent :
	public Component,
	private ComponentDragger,
	private Value::Listener
{
	public:
		SequenceComponent(ValueTree _sequenceData, Data& _data, int y, int height);
		void paint(Graphics& g) override;
		void valueChanged(Value& /*value*/) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void moved() override;
		void resized() override;

		void updateBounds();
		void updateSceneStartValueRefer();
		int getAbsoluteStart();

		enum ColourIds {
			fillColourId = 0x4934001,
			outlineColourId = 0x4934002,
		};

	private:
		ValueTree sequenceData;
		Data& data;
		Value sceneStartValue;
		SnapToGridConstrainer constrainer;
		ResizableBorderComponent resizableBorder;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceComponent)
};

#endif // SEQUENCECOMPONENT_H

#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SnapToGridConstrainer.h"

class SceneComponent : public Component,
	private Timer,
	private ComponentDragger
{
	public:
		SceneComponent(ValueTree _sceneData);
		void paint(Graphics& g) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void timerCallback() override;
		void moved() override;
		void resized() override;

		enum ColourIds {
			fillColourId = 0x2300103,
			outlineColourId = 0x2300104,
			textColourId = 0x2300105,
		};

	private:
		ValueTree sceneData;
		SnapToGridConstrainer constrainer;
		ResizableBorderComponent resizableBorder;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)
};

#endif

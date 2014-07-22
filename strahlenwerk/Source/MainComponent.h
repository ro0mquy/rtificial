#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "BoxLayout.h"
#include "OpenGLComponent.h"
#include "Timeline.h"

class MainContentComponent : public Component
{
	public:
		MainContentComponent();

		void resized() override;
		bool keyPressed(const KeyPress& key) override;

	private:
		StretchableLayoutResizerBar verticalResizer;
		Timeline timeline;

		PropertyPanel panel;
		StretchableLayoutResizerBar horizontalResizer;
		OpenGLComponent openGLComponent;

		StretchableLayoutManager verticalLayout;
		StretchableLayoutManager horizontalLayout;

		BoxLayout<3, false> horizontalBoxLayout; // <5 für <3
		BoxLayout<3, true> verticalBoxLayout;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


#endif

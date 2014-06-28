#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include <array>
#include "../JuceLibraryCode/JuceHeader.h"
#include "BoxLayout.h"
#include "OpenGLComponent.h"

class MainContentComponent   : public Component
{
public:
	MainContentComponent();

	void resized() override;

private:
	StretchableLayoutResizerBar verticalResizer;
	Component timeline;

	PropertyPanel panel;
	StretchableLayoutResizerBar horizontalResizer;
	OpenGLComponent openGLComponent;

	StretchableLayoutManager verticalLayout;
	StretchableLayoutManager horizontalLayout;

	BoxLayout<3, true> verticalBoxLayout;
	BoxLayout<3, false> horizontalBoxLayout;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif

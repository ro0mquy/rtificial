#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include <juce>
#include "BoxLayout.h"
#include "OpenGLComponent.h"
#include "Timeline/Timeline.h"
#include "LabColorPicker.h"

class MainContentComponent : public Component
{
	public:
		MainContentComponent();

		void resized() override;

	private:
		StretchableLayoutResizerBar verticalResizer;
		Timeline timeline;

		LabColorPicker colorPicker;
		StretchableLayoutResizerBar horizontalResizer;
		OpenGLComponent openGLComponent;

		StretchableLayoutManager verticalLayout;
		StretchableLayoutManager horizontalLayout;

		BoxLayout<3, false> horizontalBoxLayout; // <5 für <3
		BoxLayout<3, true> verticalBoxLayout;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


#endif

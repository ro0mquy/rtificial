#ifndef MAINCONTENTCOMPONENT_H
#define MAINCONTENTCOMPONENT_H

#include <juce>
#include "BoxLayout.h"
#include "OpenGLComponent.h"
#include "Timeline/Timeline.h"
#include "Sidebar/Sidebar.h"

class MainContentComponent : public Component
{
	public:
		MainContentComponent();

		void resized() override;

		Timeline& getTimeline();

	private:
		StretchableLayoutResizerBar verticalResizer;
		Timeline timeline;

		Sidebar sidebar;
		StretchableLayoutResizerBar horizontalResizer;
		OpenGLComponent openGLComponent;

		StretchableLayoutManager verticalLayout;
		StretchableLayoutManager horizontalLayout;

		BoxLayout<3, false> horizontalBoxLayout; // <5 für <3
		BoxLayout<3, true> verticalBoxLayout;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


#endif // MAINCONTENTCOMPONENT_H

#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "TopComponent.h"


/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainContentComponent   : public Component
{
public:
	//==============================================================================
	MainContentComponent();
	~MainContentComponent();

	void resized() override;

private:
	TopComponent topComponent;
	StretchableLayoutResizerBar resizer;
	Component timeline;

	StretchableLayoutManager layout;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif

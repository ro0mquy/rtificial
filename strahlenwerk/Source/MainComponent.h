/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OpenGLComponent.h"


//==============================================================================
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
	void childBoundsChanged(Component* child) override;

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)

	PropertyPanel panel;
	Component editWindow;
	ResizableEdgeComponent resizer;
	ComponentBoundsConstrainer constrainer;
	OpenGLComponent openGLComponent;

	Value booleanValue;
	StretchableLayoutManager layout;
};


#endif  // MAINCOMPONENT_H_INCLUDED

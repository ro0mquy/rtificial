#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include <array>
#include "../JuceLibraryCode/JuceHeader.h"
#include "TopComponent.h"

class MainContentComponent   : public Component
{
public:
	MainContentComponent();

	void resized() override;

private:
	TopComponent topComponent;
	StretchableLayoutResizerBar resizer;
	Component timeline;

	std::array<Component*, 3> components;

	StretchableLayoutManager layout;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif

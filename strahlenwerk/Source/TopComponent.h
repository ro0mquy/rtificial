#ifndef TOPCOMPONENT_H
#define TOPCOMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "OpenGLComponent.h"

class TopComponent : public Component {
	public:
		TopComponent();

		void resized() override;

	private:
		PropertyPanel panel;
		StretchableLayoutResizerBar resizer;
		OpenGLComponent openGLComponent;

		StretchableLayoutManager layout;

		Value booleanValue;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopComponent)
};

#endif

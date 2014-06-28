#ifndef TOPCOMPONENT_H
#define TOPCOMPONENT_H

#include <array>
#include "../JuceLibraryCode/JuceHeader.h"
#include "OpenGLComponent.h"
#include "BoxLayout.h"

class TopComponent : public Component {
	public:
		TopComponent();

		void resized() override;

	private:
		PropertyPanel panel;
		StretchableLayoutResizerBar resizer;
		OpenGLComponent openGLComponent;
		StretchableLayoutManager layout;

		BoxLayout<3, false> boxLayout;


		Value booleanValue;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopComponent)
};

#endif

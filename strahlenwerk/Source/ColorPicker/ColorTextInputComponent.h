#ifndef COLORTEXTINPUTCOMPONENT_H
#define COLORTEXTINPUTCOMPONENT_H

#include <juce>

class ColorTextInputComponent :
	public Component,
	public ChangeBroadcaster,
	private Label::Listener
{
	public:
		ColorTextInputComponent();
		~ColorTextInputComponent();

		void setRGB(const float R_, const float G_, const float B_);
		float getFloatR();
		float getFloatG();
		float getFloatB();

		void resized() override;
		void labelTextChanged(Label* labelThatHasChanged) override;

	private:
		float R;
		float G;
		float B;

		Label labelHex;
		Label labelInt;
		Label labelFloat;

		void updateLabels();
		void setLabelHex();
		void setLabelInt();
		void setLabelFloat();
		void getLabelHex();
		void getLabelInt();
		void getLabelFloat();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorTextInputComponent)
};

#endif // COLORTEXTINPUTCOMPONENT_H

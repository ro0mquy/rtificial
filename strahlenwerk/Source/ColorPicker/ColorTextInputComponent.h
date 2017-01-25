#ifndef COLORTEXTINPUTCOMPONENT_H
#define COLORTEXTINPUTCOMPONENT_H

#include <juce>
#include <RtColor.h>

class ColorTextInputComponent :
	public Component,
	public ChangeBroadcaster,
	private Label::Listener
{
	public:
		ColorTextInputComponent();
		~ColorTextInputComponent();

		void setCurrentColor(const RtColor& color_);
		RtColor getCurrentColor() const;

		void resized() override;
		void labelTextChanged(Label* labelThatHasChanged) override;

	private:
		RtColor color;

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

#include "LabColorPicker.h"

LabColorPicker::LabColorPicker() {
	addAndMakeVisible(colorSpace);
	addAndMakeVisible(slider);

	slider.setRange(0, 100);
	slider.setValue(50.);
	slider.addListener(this);
}

void LabColorPicker::resized() {
	const int sliderHeight = 30;
	colorSpace.setBounds(0, 0, getWidth(), getHeight() - sliderHeight);
	slider.setBounds(0, getHeight() - sliderHeight, getWidth(), sliderHeight);
}

void LabColorPicker::sliderValueChanged(Slider* slider) {
	colorSpace.setL(slider->getValue());
}

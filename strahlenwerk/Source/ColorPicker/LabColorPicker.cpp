#include "LabColorPicker.h"

LabColorPicker::LabColorPicker() :
	color(50., 0., 0.),
	colorSpace(*this, color)
{
	addAndMakeVisible(colorSpace);
	addAndMakeVisible(slider);

	slider.setRange(0, 100);
	slider.setValue(50.);
	slider.addListener(this);
}

void LabColorPicker::paint(Graphics& g) {
	g.fillAll(Colour(0, 0, 0));
	g.setColour(color.getSRGBColor());
	g.fillRect(0, 0, getWidth(), 30);
}

void LabColorPicker::resized() {
	const int sliderHeight = 30;
	const int colorHeight = 30;
	colorSpace.setBounds(0, colorHeight, getWidth(), getHeight() - sliderHeight - colorHeight);
	slider.setBounds(0, getHeight() - sliderHeight, getWidth(), sliderHeight);
}

void LabColorPicker::sliderValueChanged(Slider* slider) {
	updateL(slider->getValue());
}

void LabColorPicker::updateL(float L) {
	color.L = L;
	colorSpace.onLChanged();
	repaint();
	sendChangeMessage();
}

void LabColorPicker::updateAB(float a, float b) {
	color.a = a;
	color.b = b;
	colorSpace.onABChanged();
	repaint();
	sendChangeMessage();
}

const LabColor& LabColorPicker::getColor() const {
	return color;
}

void LabColorPicker::setColor(LabColor _color) {
	color = _color;
}

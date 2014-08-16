#include "LabColorPicker.h"

LabColorPicker::LabColorPicker() :
	color(50., 0., 0.),
	colorSpace(*this, color)
{
	addAndMakeVisible(colorSpace);
	addAndMakeVisible(lSlider);
	addAndMakeVisible(rSlider);
	addAndMakeVisible(gSlider);
	addAndMakeVisible(bSlider);

	lSlider.setRange(0, 100);
	lSlider.setValue(50.);
	lSlider.addListener(this);

	rSlider.setRange(0, 1);
	gSlider.setRange(0, 1);
	bSlider.setRange(0, 1);
	rSlider.addListener(this);
	gSlider.addListener(this);
	bSlider.addListener(this);
}

void LabColorPicker::paint(Graphics& g) {
	g.fillAll(Colour(0, 0, 0));
	g.setColour(color.getSRGBColor());
	g.fillRect(0, 0, getWidth(), 30);
}

void LabColorPicker::resized() {
	const int sliderHeight = 30;
	const int colorHeight = 30;
	colorSpace.setBounds(0, colorHeight, getWidth(), getHeight() - 4 * sliderHeight - colorHeight);
	lSlider.setBounds(0, getHeight() - 4 * sliderHeight, getWidth(), sliderHeight);
	rSlider.setBounds(0, getHeight() - 3 * sliderHeight, getWidth(), sliderHeight);
	gSlider.setBounds(0, getHeight() - 2 * sliderHeight, getWidth(), sliderHeight);
	bSlider.setBounds(0, getHeight() - 1 * sliderHeight, getWidth(), sliderHeight);
}

void LabColorPicker::sliderValueChanged(Slider* slider) {
	if(slider == &lSlider) {
		updateL(slider->getValue());
	} else if(slider == &rSlider || slider == &gSlider || slider == &bSlider) {
		color = LabColor(Colour::fromFloatRGBA(rSlider.getValue(), gSlider.getValue(), bSlider.getValue(), 1.));
		lSlider.setValue(color.L, NotificationType::dontSendNotification);
		repaint();
		colorSpace.onColorChanged();
		sendChangeMessage();
	}
}

void LabColorPicker::updateL(float L) {
	color.L = L;
	colorSpace.onLChanged();
	updateRGBSliders();
	repaint();
	sendChangeMessage();
}

void LabColorPicker::updateAB(float a, float b) {
	color.a = a;
	color.b = b;
	colorSpace.onABChanged();
	updateRGBSliders();
	repaint();
	sendChangeMessage();
}

const LabColor& LabColorPicker::getColor() const {
	return color;
}

void LabColorPicker::setColor(LabColor _color) {
	color = _color;
	updateRGBSliders();
	lSlider.setValue(color.L);
	colorSpace.onLChanged();
	repaint();
	sendChangeMessage();
}

void LabColorPicker::updateRGBSliders() {
	const auto rgb = color.getLinearRGBColor();
	rSlider.setValue(rgb.getFloatRed(), NotificationType::dontSendNotification);
	gSlider.setValue(rgb.getFloatGreen(), NotificationType::dontSendNotification);
	bSlider.setValue(rgb.getFloatBlue(), NotificationType::dontSendNotification);
}

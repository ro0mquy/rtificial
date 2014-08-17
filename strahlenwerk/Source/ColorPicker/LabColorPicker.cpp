#include "LabColorPicker.h"

LabColorPicker::LabColorPicker() :
	LabColorPicker(LabColor(50., 0., 0.))
{
}

LabColorPicker::LabColorPicker(const LabColor& color_) :
	color(color_),
	colorSpace(color)
{
	color.addListenerForLab(this);

	lSlider.setRange(0, 100);
	lSlider.getValueObject().referTo(color.L);

	rSlider.setRange(0, 1);
	gSlider.setRange(0, 1);
	bSlider.setRange(0, 1);
	rSlider.addListener(this);
	gSlider.addListener(this);
	bSlider.addListener(this);

	addAndMakeVisible(colorSpace);
	addAndMakeVisible(lSlider);
	addAndMakeVisible(rSlider);
	addAndMakeVisible(gSlider);
	addAndMakeVisible(bSlider);
}

void LabColorPicker::paint(Graphics& g) {
	g.fillAll(Colour(0, 0, 0));
	g.setColour(color.getSRGBColor());
	g.fillRect(0, 0, getWidth(), 30);
}

void LabColorPicker::resized() {
	const int sliderHeight = 24;
	const int colorHeight = 30;

	Rectangle<int> bounds = getLocalBounds();
	bounds.removeFromTop(colorHeight);
	bSlider.setBounds(bounds.removeFromBottom(sliderHeight));
	gSlider.setBounds(bounds.removeFromBottom(sliderHeight));
	rSlider.setBounds(bounds.removeFromBottom(sliderHeight));
	lSlider.setBounds(bounds.removeFromBottom(sliderHeight));
	colorSpace.setBounds(bounds);
}

void LabColorPicker::sliderValueChanged(Slider* slider) {
	color = LabColor(Colour::fromFloatRGBA(rSlider.getValue(), gSlider.getValue(), bSlider.getValue(), 1.));
	repaint();
}

void LabColorPicker::valueChanged(Value& value) {
	updateRGBSliders();
	repaint();
}

const LabColor& LabColorPicker::getColor() const {
	return color;
}

void LabColorPicker::setColor(LabColor _color) {
	color = _color;
	updateRGBSliders();
	repaint();
}

void LabColorPicker::updateRGBSliders() {
	const auto rgb = color.getLinearRGBColor();
	rSlider.setValue(rgb.getFloatRed(), NotificationType::dontSendNotification);
	gSlider.setValue(rgb.getFloatGreen(), NotificationType::dontSendNotification);
	bSlider.setValue(rgb.getFloatBlue(), NotificationType::dontSendNotification);
}

#include "ColorTextInputComponent.h"

ColorTextInputComponent::ColorTextInputComponent() {
	updateLabels();

	labelHex.addListener(this);
	labelInt.addListener(this);
	labelFloat.addListener(this);

	labelHex.setEditable(true);
	labelInt.setEditable(true);
	labelFloat.setEditable(true);

	addAndMakeVisible(labelHex);
	addAndMakeVisible(labelInt);
	addAndMakeVisible(labelFloat);
}

ColorTextInputComponent::~ColorTextInputComponent() {
	labelHex.removeListener(this);
	labelInt.removeListener(this);
	labelFloat.removeListener(this);
}

void ColorTextInputComponent::setRGB(const float R_, const float G_, const float B_) {
	R = R_;
	G = G_;
	B = B_;
	updateLabels();
}

float ColorTextInputComponent::getFloatR() {
	return R;
}

float ColorTextInputComponent::getFloatG() {
	return G;
}

float ColorTextInputComponent::getFloatB() {
	return B;
}

void ColorTextInputComponent::resized() {
	const int spacing = 20;
	const int labelHeight = 25;

	juce::Rectangle<int> rect(getLocalBounds());
	rect.reduce(spacing, spacing);

	labelHex.setBounds(rect.removeFromTop(labelHeight));

	rect.removeFromTop(spacing);
	labelInt.setBounds(rect.removeFromTop(labelHeight));

	rect.removeFromTop(spacing);
	labelFloat.setBounds(rect.removeFromTop(labelHeight));
}

void ColorTextInputComponent::labelTextChanged(Label* labelThatHasChanged) {
	if (labelThatHasChanged == &labelHex) {
		getLabelHex();
		setLabelInt();
		setLabelFloat();
	} else if (labelThatHasChanged == &labelInt) {
		getLabelInt();
		setLabelHex();
		setLabelFloat();
	} else if (labelThatHasChanged == &labelFloat) {
		getLabelFloat();
		setLabelHex();
		setLabelInt();
	}
	sendChangeMessage();
}

void ColorTextInputComponent::updateLabels() {
	setLabelHex();
	setLabelInt();
	setLabelFloat();
}

void ColorTextInputComponent::setLabelHex() {
	int hexColor = 0;
	hexColor += jlimit(0, 255, int(R * 255.f)) << 16;
	hexColor += jlimit(0, 255, int(G * 255.f)) << 8;
	hexColor += jlimit(0, 255, int(B * 255.f));

	String text(String::toHexString(hexColor));
	text = text.toUpperCase();
	text = text.paddedLeft('0', 6);
	text = "#" + text;

	labelHex.setText(text, NotificationType::dontSendNotification);
}

void ColorTextInputComponent::setLabelInt() {
	const int intR = roundFloatToInt(R * 255.f);
	const int intG = roundFloatToInt(G * 255.f);
	const int intB = roundFloatToInt(B * 255.f);

	String text = String(intR) + ", " + String(intG) + ", " + String(intB);
	labelInt.setText(text, NotificationType::dontSendNotification);
}

void ColorTextInputComponent::setLabelFloat() {
	String text = String(R, 5) + ", " + String(G, 5) + ", " + String(B, 5);
	labelFloat.setText(text, NotificationType::dontSendNotification);
}

void ColorTextInputComponent::getLabelHex() {
	String hexText = labelHex.getText();
	hexText = hexText.trimCharactersAtStart("#");
	int hexValue = hexText.getHexValue32();

	R = (hexValue >> 16 & 0xFF) / 255.f;
	G = (hexValue >> 8 & 0xFF) / 255.f;
	B = (hexValue & 0xFF) / 255.f;
}

void ColorTextInputComponent::getLabelInt() {
	String intText = labelInt.getText();
	StringArray intNumbers(StringArray::fromTokens(intText, " ,;", String::empty));
	intNumbers.removeEmptyStrings();

	R = float(intNumbers[0].getIntValue()) / 255.f;
	G = float(intNumbers[1].getIntValue()) / 255.f;
	B = float(intNumbers[2].getIntValue()) / 255.f;
}

void ColorTextInputComponent::getLabelFloat() {
	String floatText = labelFloat.getText();
	StringArray floatNumbers(StringArray::fromTokens(floatText, " ,;", String::empty));
	floatNumbers.removeEmptyStrings();

	R = floatNumbers[0].getFloatValue();
	G = floatNumbers[1].getFloatValue();
	B = floatNumbers[2].getFloatValue();
}

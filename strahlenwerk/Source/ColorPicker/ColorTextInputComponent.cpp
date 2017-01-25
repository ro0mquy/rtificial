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

void ColorTextInputComponent::setCurrentColor(const RtColor& color_) {
	color = color_;
	updateLabels();
}

RtColor ColorTextInputComponent::getCurrentColor() const {
	return color;
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
	hexColor += roundFloatToInt(color.getRed() * 255.f) << 16;
	hexColor += roundFloatToInt(color.getGreen() * 255.f) << 8;
	hexColor += roundFloatToInt(color.getBlue() * 255.f);

	String text(String::toHexString(hexColor));
	text = text.toUpperCase();
	text = text.paddedLeft('0', 6);
	text = "#" + text;

	labelHex.setText(text, NotificationType::dontSendNotification);
}

void ColorTextInputComponent::setLabelInt() {
	const int intR = roundFloatToInt(color.getRed() * 255.f);
	const int intG = roundFloatToInt(color.getGreen() * 255.f);
	const int intB = roundFloatToInt(color.getBlue() * 255.f);

	String text = String(intR) + ", " + String(intG) + ", " + String(intB);
	labelInt.setText(text, NotificationType::dontSendNotification);
}

void ColorTextInputComponent::setLabelFloat() {
	String text =
		String(color.getRed(), 5) + ", " +
		String(color.getGreen(), 5) + ", " +
		String(color.getBlue(), 5);
	labelFloat.setText(text, NotificationType::dontSendNotification);
}

void ColorTextInputComponent::getLabelHex() {
	String hexText = labelHex.getText();
	hexText = hexText.trimCharactersAtStart("#");

	const int inputLength = hexText.length();
	if (inputLength == 1) {
		hexText = String::repeatedString(hexText, 6);
	} else if (inputLength == 2) {
		hexText = String::repeatedString(hexText, 3);
	} else if (inputLength == 3) {
		hexText = String::repeatedString(String::charToString(hexText[0]), 2) + String::repeatedString(String::charToString(hexText[1]), 2) + String::repeatedString(String::charToString(hexText[2]), 2);
	}

	int hexValue = hexText.getHexValue32();

	color.setRed((hexValue >> 16 & 0xFF) / 255.f);
	color.setGreen((hexValue >> 8 & 0xFF) / 255.f);
	color.setBlue((hexValue & 0xFF) / 255.f);
}

void ColorTextInputComponent::getLabelInt() {
	String intText = labelInt.getText();
	StringArray intNumbers(StringArray::fromTokens(intText, " ,;", String::empty));
	intNumbers.removeEmptyStrings();

	if (intNumbers.size() == 1) {
		intNumbers.add(intNumbers[0]);
		intNumbers.add(intNumbers[0]);
	}

	color.setRed(float(intNumbers[0].getIntValue()) / 255.f);
	color.setGreen(float(intNumbers[1].getIntValue()) / 255.f);
	color.setBlue(float(intNumbers[2].getIntValue()) / 255.f);
}

void ColorTextInputComponent::getLabelFloat() {
	String floatText = labelFloat.getText();
	StringArray floatNumbers(StringArray::fromTokens(floatText, " ,;", String::empty));
	floatNumbers.removeEmptyStrings();

	if (floatNumbers.size() == 1) {
		floatNumbers.add(floatNumbers[0]);
		floatNumbers.add(floatNumbers[0]);
	}

	color.setRed(floatNumbers[0].getFloatValue());
	color.setGreen(floatNumbers[1].getFloatValue());
	color.setBlue(floatNumbers[2].getFloatValue());
}

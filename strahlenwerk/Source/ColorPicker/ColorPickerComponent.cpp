#include "ColorPickerComponent.h"

#include <Timeline/TimelineData.h>
#include <cmath>

ColorPickerComponent::ColorPickerComponent(Value& colorR_, Value& colorG_, Value& colorB_) :
	TabbedComponent(TabbedButtonBar::TabsAtTop),
	colorR(colorR_),
	colorG(colorG_),
	colorB(colorB_),
	hsvSelector(ColourSelector::showSliders | ColourSelector::showColourspace)
{
	setOutline(0);
	addTab("HSV & RGB", findColour(ColorPickerComponent::tabBackgroundColourId), &hsvSelector, false);
	addTab("Text", findColour(ColorPickerComponent::tabBackgroundColourId), &textSelector, false);

	updateSelectors();

	hsvSelector.addChangeListener(this);
	textSelector.addChangeListener(this);

	colorR.addListener(this);
	colorG.addListener(this);
	colorB.addListener(this);
}

ColorPickerComponent::~ColorPickerComponent() {
	hsvSelector.removeChangeListener(this);
	textSelector.removeChangeListener(this);

	colorR.removeListener(this);
	colorG.removeListener(this);
	colorB.removeListener(this);
}

void ColorPickerComponent::setValueData(Value& colorR_, Value& colorG_, Value& colorB_) {
	colorR.referTo(colorR_);
	colorG.referTo(colorG_);
	colorB.referTo(colorB_);
}

void ColorPickerComponent::currentTabChanged(int /*newCurrentTabIndex*/, const String& /*newCurrentTabName*/) {
	updateSelectors();
}

void ColorPickerComponent::valueChanged(Value& /*value*/) {
	// color* changed
	updateSelectors();
}

void ColorPickerComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &hsvSelector) {
		getHsvSelector();
	} else if (source == &textSelector) {
		getTextSelector();
	}
}

void ColorPickerComponent::updateSelectors() {
	const Component* currentComp = getCurrentContentComponent();

	if (currentComp == &hsvSelector) {
		setHsvSelector();
	} else if (currentComp == &textSelector) {
		setTextSelector();
	}
}

void ColorPickerComponent::setHsvSelector() {
	float r = colorR.getValue();
	float g = colorG.getValue();
	float b = colorB.getValue();

	// perform gamma correction
	// TODO: real srgb conversion
	r = std::pow(r, 1.f/2.2f);
	g = std::pow(g, 1.f/2.2f);
	b = std::pow(b, 1.f/2.2f);

	// update hsvSelector
	const uint8 rInt = roundFloatToInt(r * 255.f);
	const uint8 gInt = roundFloatToInt(g * 255.f);
	const uint8 bInt = roundFloatToInt(b * 255.f);
	hsvSelector.setCurrentColour(Colour(rInt, gInt, bInt));
}

void ColorPickerComponent::setTextSelector() {
	const float r = colorR.getValue();
	const float g = colorG.getValue();
	const float b = colorB.getValue();

	// update textSelector
	textSelector.setRGB(r, g, b);
}

void ColorPickerComponent::getHsvSelector() {
	const Colour rgb = hsvSelector.getCurrentColour();
	// perform gamma correction
	// TODO: real srgb conversion
	colorR = std::pow(rgb.getFloatRed(), 2.2f);
	colorG = std::pow(rgb.getFloatGreen(), 2.2f);
	colorB = std::pow(rgb.getFloatBlue(), 2.2f);
}

void ColorPickerComponent::getTextSelector() {
	colorR = textSelector.getFloatR();
	colorG = textSelector.getFloatG();
	colorB = textSelector.getFloatB();
}

#include "ColorPickerComponent.h"

#include <Timeline/TimelineData.h>

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
		const Colour rgb = hsvSelector.getCurrentColour();
		colorR = rgb.getFloatRed();
		colorG = rgb.getFloatGreen();
		colorB = rgb.getFloatBlue();
	} else if (source == &textSelector) {
		colorR = textSelector.getFloatR();
		colorG = textSelector.getFloatG();
		colorB = textSelector.getFloatB();
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
	const float r = colorR.getValue();
	const float g = colorG.getValue();
	const float b = colorB.getValue();

	// update hsvSelector
	const uint8 rInt = r * 255;
	const uint8 gInt = g * 255;
	const uint8 bInt = b * 255;
	hsvSelector.setCurrentColour(Colour(rInt, gInt, bInt));
}

void ColorPickerComponent::setTextSelector() {
	const float r = colorR.getValue();
	const float g = colorG.getValue();
	const float b = colorB.getValue();

	// update textSelector
	textSelector.setRGB(r, g, b);
}

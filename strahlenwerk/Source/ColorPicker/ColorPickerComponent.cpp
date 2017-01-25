#include "ColorPickerComponent.h"

#include <Timeline/TimelineData.h>
#include <cmath>

ColorPickerComponent::ColorPickerComponent(Value& colorR_, Value& colorG_, Value& colorB_) :
	TabbedComponent(TabbedButtonBar::TabsAtTop),
	colorR(colorR_),
	colorG(colorG_),
	colorB(colorB_),
	hsvSelector(ColourSelector::showSliders | ColourSelector::showColourspace),
	rtSelector()
{
	setOutline(0);
	addTab("HCY", findColour(ColorPickerComponent::tabBackgroundColourId), &rtSelector, false);
	addTab("HSV & RGB", findColour(ColorPickerComponent::tabBackgroundColourId), &hsvSelector, false);
	addTab("Text", findColour(ColorPickerComponent::tabBackgroundColourId), &textSelector, false);

	updateSelectors();

	hsvSelector.addChangeListener(this);
	rtSelector.addChangeListener(this);
	textSelector.addChangeListener(this);

	colorR.addListener(this);
	colorG.addListener(this);
	colorB.addListener(this);
}

ColorPickerComponent::~ColorPickerComponent() {
	hsvSelector.removeChangeListener(this);
	rtSelector.removeChangeListener(this);
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
	} else if (source == &rtSelector) {
		getRtSelector();
	} else if (source == &textSelector) {
		getTextSelector();
	}
}

void ColorPickerComponent::updateSelectors() {
	const Component* currentComp = getCurrentContentComponent();

	if (currentComp == &hsvSelector) {
		setHsvSelector();
	} else if (currentComp == &rtSelector) {
		setRtSelector();
	} else if (currentComp == &textSelector) {
		setTextSelector();
	}
}

void ColorPickerComponent::setHsvSelector() {
	const RtColor linear(colorR.getValue(), colorG.getValue(), colorB.getValue());
	const RtColor srgb = linear.toSrgb();
	hsvSelector.setCurrentColour(srgb);
}

void ColorPickerComponent::setRtSelector() {
	const RtColor linear(colorR.getValue(), colorG.getValue(), colorB.getValue());
	const RtColor srgb = linear.toSrgb();
	rtSelector.setCurrentColor(srgb);
}

void ColorPickerComponent::setTextSelector() {
	const RtColor linear(colorR.getValue(), colorG.getValue(), colorB.getValue());
	const RtColor srgb = linear.toSrgb();
	textSelector.setCurrentColor(srgb);
}

void ColorPickerComponent::getHsvSelector() {
	const RtColor srgb = hsvSelector.getCurrentColour();
	const RtColor linear = srgb.toLinear();

	colorR = linear.getRed();
	colorG = linear.getGreen();
	colorB = linear.getBlue();
}

void ColorPickerComponent::getRtSelector() {
	const RtColor srgb = rtSelector.getCurrentColor();
	const RtColor linear = srgb.toLinear();

	colorR = linear.getRed();
	colorG = linear.getGreen();
	colorB = linear.getBlue();
}

void ColorPickerComponent::getTextSelector() {
	const RtColor srgb = textSelector.getCurrentColor();
	const RtColor linear = srgb.toLinear();

	colorR = linear.getRed();
	colorG = linear.getGreen();
	colorB = linear.getBlue();
}

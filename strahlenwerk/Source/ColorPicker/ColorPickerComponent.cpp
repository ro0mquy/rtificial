#include "ColorPickerComponent.h"

#include <Timeline/TimelineData.h>

ColorPickerComponent::ColorPickerComponent(Value& colorR_, Value& colorG_, Value& colorB_) :
	TabbedComponent(TabbedButtonBar::TabsAtTop),
	colorR(colorR_),
	colorG(colorG_),
	colorB(colorB_),
	hsvSelector(ColourSelector::showColourAtTop | ColourSelector::showSliders | ColourSelector::showColourspace)
{
	setOutline(0);
	addTab("HSV", findColour(ColorPickerComponent::tabBackgroundColourId), &hsvSelector, false);

	const uint8 r = (float) colorR.getValue() * 255;
	const uint8 g = (float) colorG.getValue() * 255;
	const uint8 b = (float) colorB.getValue() * 255;
	hsvSelector.setCurrentColour(Colour(r, g, b));
	hsvSelector.addChangeListener(this);
	hsvSelector.addMouseListener(this, true);

	colorR.addListener(this);
	colorG.addListener(this);
	colorB.addListener(this);
}

ColorPickerComponent::~ColorPickerComponent() {
	hsvSelector.removeChangeListener(this);
	hsvSelector.removeMouseListener(this);
	colorR.removeListener(this);
	colorG.removeListener(this);
	colorB.removeListener(this);
}

void ColorPickerComponent::setValueData(Value& colorR_, Value& colorG_, Value& colorB_) {
	colorR.referTo(colorR_);
	colorG.referTo(colorG_);
	colorB.referTo(colorB_);
}

void ColorPickerComponent::valueChanged(Value& /*value*/) {
	// color* changed
	const uint8 r = (float) colorR.getValue() * 255;
	const uint8 g = (float) colorG.getValue() * 255;
	const uint8 b = (float) colorB.getValue() * 255;
	hsvSelector.setCurrentColour(Colour(r, g, b));
}

void ColorPickerComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// hsvSelector changed
	const Colour rgb = hsvSelector.getCurrentColour();
	colorR = rgb.getFloatRed();
	colorG = rgb.getFloatGreen();
	colorB = rgb.getFloatBlue();
}

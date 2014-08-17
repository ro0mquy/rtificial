#ifndef LABCOLORPICKER_H
#define LABCOLORPICKER_H

#include <juce>

#include "LabColorSpaceView.h"

class LabColorPicker :
	public Component,
	private Value::Listener,
	private Slider::Listener
{
	public:
		LabColorPicker();
		LabColorPicker(const LabColor& color_);

		void paint(Graphics& g) override;
		void resized() override;
		void sliderValueChanged(Slider* slider) override;
		void valueChanged(Value& value) override;

		const LabColor& getColor() const;
		void setColor(LabColor color);

	private:
		void updateRGBSliders();

		LabColor color;

		LabColorSpaceView colorSpace;
		Slider lSlider;
		Slider rSlider;
		Slider gSlider;
		Slider bSlider;
};

#endif

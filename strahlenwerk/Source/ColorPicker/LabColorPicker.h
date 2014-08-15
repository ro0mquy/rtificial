#ifndef LABCOLORPICKER_H
#define LABCOLORPICKER_H

#include <juce>

#include "LabColorSpaceView.h"
#include "ColorChangedListener.h"

class LabColorPicker :
	public Component,
	public ColorChangedListener,
	private SliderListener
{
	public:
		LabColorPicker();

		void paint(Graphics& g) override;
		void resized() override;

		void sliderValueChanged(Slider* slider) override;

		void updateL(float L) override;
		void updateAB(float a, float b) override;

	private:
		LabColorSpaceView colorSpace;
		Slider slider;

		LabColor color;
};

#endif

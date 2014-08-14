#ifndef LABCOLORPICKER_H
#define LABCOLORPICKER_H

#include <juce>

#include "LabColorSpaceView.h"

class LabColorPicker : public Component, private SliderListener {
	public:
		LabColorPicker();

		void resized() override;
		void sliderValueChanged(Slider* slider) override;

	private:
		LabColorSpaceView colorSpace;
		Slider slider;

};

#endif

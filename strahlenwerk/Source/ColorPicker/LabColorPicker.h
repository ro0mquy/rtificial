#ifndef LABCOLORPICKER_H
#define LABCOLORPICKER_H

#include <juce>

#include "LabColorSpaceView.h"
#include "ColorChangedListener.h"

class LabColorPicker :
	public Component,
	public ColorChangedListener,
	public ChangeBroadcaster,
	private SliderListener
{
	public:
		LabColorPicker();

		void paint(Graphics& g) override;
		void resized() override;

		void sliderValueChanged(Slider* slider) override;

		void updateL(float L) override;
		void updateAB(float a, float b) override;

		const LabColor& getColor() const;
		void setColor(LabColor color);

	private:
		LabColor color;

		LabColorSpaceView colorSpace;
		Slider slider;
};

#endif

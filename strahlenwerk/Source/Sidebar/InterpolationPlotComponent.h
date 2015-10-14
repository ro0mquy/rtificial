#ifndef INTERPOLATION_PLOT_COMPONENT_H
#define INTERPOLATION_PLOT_COMPONENT_H

#include <juce>

class TimelineData;

class InterpolationPlotComponent :
	public Component
{
	public:
		InterpolationPlotComponent(ValueTree sequence);

		void paint(Graphics& g) override;

		enum ColourIds {
			backgroundColourId = 0x35c2a01,
			axisLineColourId,
			plotLineColourId,
			keyPointColourId,
		};

	private:
		ValueTree sequence;
		TimelineData& data;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InterpolationPlotComponent)
};

#endif

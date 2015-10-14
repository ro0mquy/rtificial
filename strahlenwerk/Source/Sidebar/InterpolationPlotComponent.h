#ifndef INTERPOLATION_PLOT_COMPONENT_H
#define INTERPOLATION_PLOT_COMPONENT_H

#include <juce>
#include <Timeline/TimelineData.h>

class InterpolationPlotComponent :
	public Component
{
	public:
		InterpolationPlotComponent(ValueTree sequence);
		~InterpolationPlotComponent();

		void paint(Graphics& g) override;

		enum ColourIds {
			backgroundColourId = 0x35c2a01,
			axisLineColourId = 0x35c2a02,
			plotLineColourId = 0x35c2a03,
			keyPointColourId = 0x35c2a04,
		};

	private:
		ValueTree sequence;
		TimelineData& data;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InterpolationPlotComponent);
};

#endif

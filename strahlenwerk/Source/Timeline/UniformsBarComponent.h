#ifndef UNIFORMSBARCOMPONENT_H
#define UNIFORMSBARCOMPONENT_H

#include <juce>

class TimelineData;

class UniformsBarComponent : public Component {
	public:
		UniformsBarComponent();
		void updateSize();
		void paint(Graphics& g) override;
		void mouseUp(const MouseEvent& event) override;

		enum ColourIds{
			evenRowColourId = 0x2300301,
			oddRowColourId = 0x2300302,
			uniformTextColourId = 0x2300303,
			seperatorColourId = 0x2300304
		};

	private:
		TimelineData& data;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UniformsBarComponent)
};

#endif // UNIFORMSBARCOMPONENT_H

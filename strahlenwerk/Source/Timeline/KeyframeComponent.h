#ifndef KEYFRAMECOMPONENT_H
#define KEYFRAMECOMPONENT_H

#include <juce>

class KeyframeComponent :
	public Component,
	private ComponentDragger
{
	public:
		KeyframeComponent(ValueTree keyframeData_);

		void paint(Graphics& g) override;

		void updateBounds();

		enum ColourIds {
			fillColourId = 0x9414001,
		};

	private:
		ValueTree keyframeData;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyframeComponent)
};

#endif // KEYFRAMECOMPONENT_H

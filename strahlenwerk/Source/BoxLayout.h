#ifndef BOXLAYOUT_H
#define BOXLAYOUT_H

#include <array>
#include "../JuceLibraryCode/JuceHeader.h"

#include <iostream>
using namespace std;

template <int N, bool VERTICAL>
class BoxLayout : public Component {
	public:
		BoxLayout(StretchableLayoutManager& layoutManager, std::array<Component*, N>&& components) :
			layoutManager(layoutManager),
			components(components)
		{
			for(Component* component : components) {
				addAndMakeVisible(*component);
			}
		}

		void resized() override {
			layoutManager.layOutComponents(components.data(), N, 0, 0, getWidth(), getHeight(), VERTICAL, true);
		}

	private:
		StretchableLayoutManager& layoutManager;
		std::array<Component*, N> components;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BoxLayout)
};

#endif

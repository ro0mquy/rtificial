#ifndef BOXLAYOUT_H
#define BOXLAYOUT_H

#include <array>
#include <juce>

template <int N, bool VERTICAL, bool RESIZE = true>
class BoxLayout : public Component {
	public:
		BoxLayout(StretchableLayoutManager& layoutManager_, std::array<Component*, N>&& components_) :
			layoutManager(layoutManager_),
			components(components_)
		{
			for(Component* component : components) {
				addAndMakeVisible(*component);
			}
		}

		void resized() override {
			layoutManager.layOutComponents(components.data(), N, 0, 0, getWidth(), getHeight(), VERTICAL, RESIZE);
		}

	private:
		StretchableLayoutManager& layoutManager;
		std::array<Component*, N> components;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoxLayout)
};

#endif

#include "TopComponent.h"

class MakeDemoButton : public ButtonPropertyComponent {
	public:
		MakeDemoButton() : ButtonPropertyComponent("dings", false) {
		}

		void buttonClicked() override {
		}

		String getButtonText() const override {
			return "Make demo";
		}
};

TopComponent::TopComponent() :
	boxLayout(layout, { &panel, &resizer, &openGLComponent }),
	resizer(&layout, 1, true) {

	// Array<PropertyComponent*> properties;
	// properties.add(new BooleanPropertyComponent(booleanValue, "Zustand des Dingses", "Dings aus"));
	// properties.add(new MakeDemoButton());
	// // transfers ownership of properties
	// panel.addSection(properties);


	addAndMakeVisible(boxLayout);
}

void TopComponent::resized() {
	boxLayout.setBounds(0, 0, getWidth(), getHeight());
}

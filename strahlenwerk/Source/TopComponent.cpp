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
	resizer(&layout, 1, true),
	components({ &panel, &resizer, &openGLComponent }) {

	Array<PropertyComponent*> properties;
	properties.add(new BooleanPropertyComponent(booleanValue, "Zustand des Dingses", "Dings aus"));
	properties.add(new MakeDemoButton());
	// transfers ownership of properties
	panel.addProperties(properties);

	layout.setItemLayout(0, 200, -1., -.4);
	layout.setItemLayout(1, 8, 8, 8);
	layout.setItemLayout(2, 400, -1., -.6);

	for(Component* component : components) {
		addAndMakeVisible(*component);
	}

	resized();
}

void TopComponent::resized() {
	layout.layOutComponents(components.data(), 3, 0, 0, getWidth(), getHeight(), false, true);
}

/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

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

//==============================================================================
MainContentComponent::MainContentComponent() :
	resizer(&layout, 1, true) {
	setSize (800, 600);

	Array<PropertyComponent*> properties;
	properties.add(new BooleanPropertyComponent(booleanValue, "Zustand des Dingses", "Dings aus"));
	properties.add(new MakeDemoButton());
	panel.addProperties(properties);

	layout.setItemLayout(0, 200, -1., -.4);
	layout.setItemLayout(1, 8, 8, 8);
	layout.setItemLayout(2, 400, -1., -.6);

	addAndMakeVisible(panel);
	addAndMakeVisible(resizer);
	addAndMakeVisible(openGLComponent);

	resized();
}

MainContentComponent::~MainContentComponent() {
}

void MainContentComponent::resized() {
	Component* comps[] = { &panel, &resizer, &openGLComponent };
	layout.layOutComponents(comps, 3, 0, 0, getWidth(), getHeight(), false, true);
}

void MainContentComponent::childBoundsChanged(Component* child) {
	if(child == &panel) {
		resized();
	}
}

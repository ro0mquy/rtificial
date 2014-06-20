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
	resizer(&panel, &constrainer, ResizableEdgeComponent::rightEdge) {
	setSize (500, 400);

	Array<PropertyComponent*> components;
	components.add(new BooleanPropertyComponent(booleanValue, "Zustand des Dingses", "Dings aus"));
	components.add(new MakeDemoButton());
	panel.addProperties(components);

	addAndMakeVisible(panel);
	panel.addAndMakeVisible(resizer);
	resizer.setAlwaysOnTop(true);

	panel.setBounds(0, 0, 300, getHeight());

	addAndMakeVisible(openGLComponent);
	openGLComponent.setBounds(300, 0, 200, getHeight());

	resized();
}

MainContentComponent::~MainContentComponent() {
}

void MainContentComponent::resized() {
	auto b(panel.getLocalBounds());
	panel.setBounds(b.withHeight(getHeight()));

	auto c(openGLComponent.getLocalBounds());
	openGLComponent.setBounds(c.withHeight(getHeight()) + b.getTopRight());

	Rectangle<int> r(getLocalBounds());
	r.removeFromLeft(panel.getWidth() - 4);
	resizer.setBounds(r.withWidth(4));

}

void MainContentComponent::childBoundsChanged(Component* child) {
	if(child == &panel) {
		resized();
	}
}

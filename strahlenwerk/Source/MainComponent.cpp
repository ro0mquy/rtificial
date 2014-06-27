#include "MainComponent.h"

MainContentComponent::MainContentComponent() :
	resizer(&layout, 1, false),
	components({{ &topComponent, &resizer, &timeline }}) {

	setSize (800, 600);

	layout.setItemLayout(0, 200, -1., -.66);
	layout.setItemLayout(1, 8, 8, 8);
	layout.setItemLayout(2, 200, -1., -.33 );

	for(Component* component : components) {
		addAndMakeVisible(*component);
	}

	resized();
}

void MainContentComponent::resized() {
	layout.layOutComponents(components.data(), 3, 0, 0, getWidth(), getHeight(), true, true);
}

#include "MainComponent.h"

MainContentComponent::MainContentComponent() :
	resizer(&layout, 1, false) {

	setSize (800, 600);

	layout.setItemLayout(0, 200, -1., -.66);
	layout.setItemLayout(1, 8, 8, 8);
	layout.setItemLayout(2, 200, -1., -.33 );

	addAndMakeVisible(topComponent);
	addAndMakeVisible(resizer);
	addAndMakeVisible(timeline);

	resized();
}

MainContentComponent::~MainContentComponent() {
}

void MainContentComponent::resized() {
	Component* components[] = { &topComponent, &resizer, &timeline };
	layout.layOutComponents(components, 3, 0, 0, getWidth(), getHeight(), true, true);
}

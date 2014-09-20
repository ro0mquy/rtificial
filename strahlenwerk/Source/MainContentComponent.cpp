#include "MainContentComponent.h"

MainContentComponent::MainContentComponent() :
	verticalResizer(&verticalLayout, 1, false),
	horizontalResizer(&horizontalLayout, 1, true),
	horizontalBoxLayout(horizontalLayout, {{ &sidebar, &horizontalResizer, &openGLComponent }}),
	verticalBoxLayout(verticalLayout, {{ &horizontalBoxLayout, &verticalResizer, &timeline }})
{

	setSize(800, 600);

	horizontalLayout.setItemLayout(0, 200, -1., -.3);
	horizontalLayout.setItemLayout(1, 8, 8, 8);
	horizontalLayout.setItemLayout(2, 400, -1., -.7);

	verticalLayout.setItemLayout(0, 200, -1., -.66);
	verticalLayout.setItemLayout(1, 8, 8, 8);
	verticalLayout.setItemLayout(2, 200, -1., -.33 );

	addAndMakeVisible(verticalBoxLayout);
}

void MainContentComponent::resized() {
	verticalBoxLayout.setBounds(getLocalBounds());
}

Timeline& MainContentComponent::getTimeline() {
	return timeline;
}

void MainContentComponent::repaintOpenGLComponent() {
	openGLComponent.repaintChildren();
}

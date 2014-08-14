#include "MainComponent.h"

MainContentComponent::MainContentComponent() :
	verticalResizer(&verticalLayout, 1, false),
	horizontalResizer(&horizontalLayout, 1, true),
	horizontalBoxLayout(horizontalLayout, {{ &colorPicker, &horizontalResizer, &openGLComponent }}),
	verticalBoxLayout(verticalLayout, {{ &horizontalBoxLayout, &verticalResizer, &timeline }})
{

	setSize(800, 600);

	horizontalLayout.setItemLayout(0, 200, -1., -.4);
	horizontalLayout.setItemLayout(1, 8, 8, 8);
	horizontalLayout.setItemLayout(2, 400, -1., -.6);

	verticalLayout.setItemLayout(0, 200, -1., -.66);
	verticalLayout.setItemLayout(1, 8, 8, 8);
	verticalLayout.setItemLayout(2, 200, -1., -.33 );

	addAndMakeVisible(verticalBoxLayout);
}

void MainContentComponent::resized() {
	verticalBoxLayout.setBounds(getLocalBounds());
}

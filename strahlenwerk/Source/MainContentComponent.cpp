#include "MainContentComponent.h"
#include <StrahlenwerkApplication.h>
#include <PropertyNames.h>
#include <RtificialLookAndFeel.h>

MainContentComponent::MainContentComponent() :
	verticalResizer(&verticalLayout, 1, false),
	horizontalResizer(&horizontalLayout, 1, true),
	horizontalBoxLayout(horizontalLayout, {{ &sidebar, &horizontalResizer, &openGLComponent }}),
	verticalBoxLayout(verticalLayout, {{ &horizontalBoxLayout, &verticalResizer, &timeline }})
{

	setSize(800, 600);

	setDefaultLayout();

	addAndMakeVisible(verticalBoxLayout);
}

void MainContentComponent::resized() {
	verticalBoxLayout.setBounds(getLocalBounds());
}

Timeline& MainContentComponent::getTimeline() {
	return timeline;
}

void MainContentComponent::setDefaultLayout() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const int glComponentWidth = properties.getIntValue(PropertyNames::OpenGLWidth, 400);
	const int glComponentHeight = properties.getIntValue(PropertyNames::OpenGLHeight, 300);

	horizontalLayout.setItemLayout(0, 200, -1., getWidth() - glComponentWidth - 8);
	horizontalLayout.setItemLayout(1, 8, 8, 8);
	horizontalLayout.setItemLayout(2, 160, -1., glComponentWidth);

	verticalLayout.setItemLayout(0, 90, -1., glComponentHeight);
	verticalLayout.setItemLayout(1, 8, 8, 8);
	verticalLayout.setItemLayout(2, 200, -1., getHeight() - glComponentHeight - 8 );
	verticalBoxLayout.resized();
}

void MainContentComponent::setOpenGLOnlyLayout() {
	horizontalLayout.setItemLayout(0, 0, 0, 0);
	horizontalLayout.setItemLayout(1, 0, 0, 0);
	horizontalLayout.setItemLayout(2, -1, -1, -1);

	verticalLayout.setItemLayout(0, -1, -1, -1);
	verticalLayout.setItemLayout(1, 0, 0, 0);
	verticalLayout.setItemLayout(2, 0, 0, 0);
	verticalBoxLayout.resized();
}

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

Sidebar& MainContentComponent::getSidebar() {
	return sidebar;
}

OpenGLComponent& MainContentComponent::getOpenGLComponent() {
	return openGLComponent;
}

void MainContentComponent::setDefaultLayout() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const double glComponentWidth = properties.getDoubleValue(PropertyNames::OpenGLWidth, .5);
	const double glComponentHeight = properties.getDoubleValue(PropertyNames::OpenGLHeight, .5);

	horizontalLayout.setItemLayout(0, 200, -1., getWidth() * (1. - glComponentWidth) - RtificialLookAndFeel::strechableLayoutResizerBarWidth);
	horizontalLayout.setItemLayout(1, RtificialLookAndFeel::strechableLayoutResizerBarWidth, RtificialLookAndFeel::strechableLayoutResizerBarWidth, RtificialLookAndFeel::strechableLayoutResizerBarWidth);
	horizontalLayout.setItemLayout(2, 160, -1., -glComponentWidth);

	verticalLayout.setItemLayout(0, 90, -1., -glComponentHeight);
	verticalLayout.setItemLayout(1, RtificialLookAndFeel::strechableLayoutResizerBarWidth, RtificialLookAndFeel::strechableLayoutResizerBarWidth, RtificialLookAndFeel::strechableLayoutResizerBarWidth);
	verticalLayout.setItemLayout(2, 200, -1., getHeight() * (1. - glComponentHeight) - RtificialLookAndFeel::strechableLayoutResizerBarWidth);
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

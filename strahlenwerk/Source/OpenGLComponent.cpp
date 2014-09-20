#include "MainWindow.h"

#include "OpenGLComponent.h"

OpenGLComponent::OpenGLComponent() :
	renderer(context)
{
	MainWindow::getApplicationCommandManager().addListener(this);
	setOpaque(true);
	fixedAspectRatioComponent.setOpaque(true);

	// create a component with a fixed aspect ratio
	// it will get resized to full size by setBoundsToFit()
	fixedAspectRatioComponent.setSize(16, 9);
	addAndMakeVisible(fixedAspectRatioComponent);

	context.setRenderer(&renderer);
	context.attachTo(fixedAspectRatioComponent);
	context.setContinuousRepainting(true);
}

OpenGLComponent::~OpenGLComponent() {
	context.detach();
}

void OpenGLComponent::repaintChildren() {
	repaint();
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::resized() {
	fixedAspectRatioComponent.setBoundsToFit(0, 0, getWidth(), getHeight(), Justification(Justification::centred), false);
	renderer.setSize(fixedAspectRatioComponent.getWidth(), fixedAspectRatioComponent.getHeight());
}

void OpenGLComponent::paint(Graphics& g) {
	g.fillAll(Colours::black);
}

void OpenGLComponent::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	if(info.commandID == MainWindow::quitProgram) {
		context.detach();
	}
}

void OpenGLComponent::applicationCommandListChanged() {
}

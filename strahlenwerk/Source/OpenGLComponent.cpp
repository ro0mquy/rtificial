#include "OpenGLComponent.h"
#include <MainWindow.h>
#include <Timeline/TimelineData.h>
#include <Timeline/SpecialUniformController.h>
#include <AudioManager.h>
#include <StrahlenwerkApplication.h>
#include <Project/Project.h>

OpenGLComponent::OpenGLComponent() :
	renderer(context),
	fixedAspectRatioComponent(renderer)
{
	// TODO: remove listeners again
	MainWindow::getApplicationCommandManager().addListener(this);
	TimelineData::getTimelineData().addListenerToTree(this);
	CameraController::globalCameraController->addChangeListener(this);
	AudioManager::getAudioManager().addChangeListener(this);
	StrahlenwerkApplication::getInstance()->getProject().registerListener(this);

	setOpaque(true);

	// create a component with a fixed aspect ratio
	// it will get resized to full size by setBoundsToFit()
	fixedAspectRatioComponent.setSize(16, 9);
	addAndMakeVisible(fixedAspectRatioComponent);

	context.setRenderer(&renderer);
	context.attachTo(fixedAspectRatioComponent);
}

OpenGLComponent::~OpenGLComponent() {
	context.detach();
}

void OpenGLComponent::makeDemo() {
	renderer.makeDemo();
}

void OpenGLComponent::repaintChildren() {
	repaint();
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::resized() {
	fixedAspectRatioComponent.setSize(16, 9);
	fixedAspectRatioComponent.setBoundsToFit(0, 0, getWidth(), getHeight(), Justification(Justification::centred), false);
	renderer.setSize(fixedAspectRatioComponent.getWidth(), fixedAspectRatioComponent.getHeight());
}

void OpenGLComponent::paint(Graphics& g) {
	g.fillAll(Colours::black);
}

void OpenGLComponent::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch(info.commandID) {
		case MainWindow::quitProgram:
			context.detach();
			break;
		case MainWindow::makeDemo:
			makeDemo();
			break;
		default:
			break;
	}
}

void OpenGLComponent::applicationCommandListChanged() {
}

void OpenGLComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// time or camera changed
	fixedAspectRatioComponent.repaint();
	// it seems that a repaint of the target component also repaints the gl context
	//context.triggerRepaint();
}

void OpenGLComponent::postprocChanged() {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::scenesChanged() {
	fixedAspectRatioComponent.repaint();
}

// TODO: better checking if a repaint is necessary
// ValueTree::Listener callbacks
void OpenGLComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	fixedAspectRatioComponent.repaint();
}

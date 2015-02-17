#include "OpenGLComponent.h"
#include <MainWindow.h>
#include <Timeline/TimelineData.h>
#include <Timeline/SpecialUniformController.h>
#include <AudioManager.h>
#include <StrahlenwerkApplication.h>
#include <Project/Project.h>
#include <PropertyNames.h>

OpenGLComponent::OpenGLComponent() :
	renderer(context),
	fixedAspectRatioComponent(renderer)
{
	// TODO: remove listeners again
	MainWindow::getApplicationCommandManager().addListener(this);
	TimelineData::getTimelineData().addListenerToTree(this);
	CameraController::globalCameraController->addChangeListener(this);
	AudioManager::getAudioManager().addChangeListener(this);
	StrahlenwerkApplication::getInstance()->getProject().addListener(this);

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

void OpenGLComponent::resized() {
	fixedAspectRatioComponent.setSize(16, 9);
	fixedAspectRatioComponent.setBoundsToFit(0, 0, getWidth(), getHeight(), Justification(Justification::centred), false);
	renderer.setSize(fixedAspectRatioComponent.getWidth(), fixedAspectRatioComponent.getHeight());

	if (Desktop::getInstance().getKioskModeComponent() == nullptr) {
		auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
		const Component* mainContent = findParentComponentOfClass<MainContentComponent>();
		if (mainContent != nullptr) {
			properties.setValue(PropertyNames::OpenGLWidth, double(getWidth()) / double(mainContent->getWidth()));
			properties.setValue(PropertyNames::OpenGLHeight, double(getHeight()) / double(mainContent->getHeight()));
		}
	}
}

void OpenGLComponent::paint(Graphics& g) {
	g.fillAll(Colours::black);
}

void OpenGLComponent::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case MainWindow::quitProgram:
			context.detach();
			break;
		case OpenGLComponent::toggleGrid:
			doToggleGrid();
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

void OpenGLComponent::doToggleGrid() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::GRID_ENABLED);
	properties.setValue(PropertyNames::GRID_ENABLED, !previous);
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

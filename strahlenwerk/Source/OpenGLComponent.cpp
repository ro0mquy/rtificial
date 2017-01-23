#include "OpenGLComponent.h"
#include <MainWindow.h>
#include <Timeline/TimelineData.h>
#include <Timeline/CameraController.h>
#include <AudioManager.h>
#include <StrahlenwerkApplication.h>
#include <Project/Project.h>
#include <PropertyNames.h>

OpenGLComponent::OpenGLComponent() :
	renderer(context),
	fixedAspectRatioComponent(renderer),
	screenRecorder(renderer)
{
	// TODO: remove listeners again
	MainWindow::getApplicationCommandManager().addListener(this);
	TimelineData::getTimelineData().addListenerToTree(this);
	TimelineData::getTimelineData().getSelection().addChangeListener(this);
	CameraController::globalCameraController->addChangeListener(this);
	AudioManager::getAudioManager().addChangeListener(this);
	StrahlenwerkApplication::getInstance()->getProject().addListener(this);

	setOpaque(true);

	// create a component with a fixed aspect ratio
	// it will get resized to full size by setBoundsToFit()
	fixedAspectRatioComponent.setSize(16, 9);
	addAndMakeVisible(fixedAspectRatioComponent);

	const bool overlaysEnabled = StrahlenwerkApplication::getInstance()->getProperties().getBoolValue(PropertyNames::OpenGLOverlaysEnabled);
	context.setComponentPaintingEnabled(overlaysEnabled);

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
		case OpenGLComponent::toggleGlOverlays:
			doToggleGlOverlays();
			break;
	}
}

void OpenGLComponent::applicationCommandListChanged() {
}

void OpenGLComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// time, selection or camera changed
	triggerRepaint();
}

void OpenGLComponent::triggerRepaint(){
	fixedAspectRatioComponent.repaint();
	// it seems that a repaint of the target component also repaints the gl context
	//context.triggerRepaint();
}

void OpenGLComponent::postprocChanged() {
	triggerRepaint();
}

void OpenGLComponent::scenesChanged() {
	triggerRepaint();
}

void OpenGLComponent::doToggleGrid() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::GridEnabled);
	properties.setValue(PropertyNames::GridEnabled, !previous);
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::doToggleGlOverlays() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::GridEnabled);
	const bool updated = not previous;
	properties.setValue(PropertyNames::GridEnabled, updated);

	context.setComponentPaintingEnabled(updated);

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

void OpenGLComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
	fixedAspectRatioComponent.repaint();
}

void OpenGLComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	fixedAspectRatioComponent.repaint();
}

Renderer& OpenGLComponent::getRenderer() {
	return renderer;
}

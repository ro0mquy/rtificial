#include "ScopesComponent.h"

#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <Renderer.h>
#include <Timeline/TimelineData.h>
#include <Timeline/CameraController.h>
#include <AudioManager.h>

ScopesComponent::ScopesComponent() :
	renderer(nullptr),
	data(TimelineData::getTimelineData()),
	audioManager(AudioManager::getAudioManager())
{
	histogramComponent = new HistogramComponent(image);

	concertinaPanel = new ConcertinaPanel();
	concertinaPanel->addPanel(0, histogramComponent, false);
	addAndMakeVisible(concertinaPanel);

	if (isVisible()) {
		addAsListener();
	}

	triggerAsyncUpdate();
}

void ScopesComponent::handleAsyncUpdate() {
	renderer = & StrahlenwerkApplication::getInstance()->getMainWindow().getMainContentComponent().getOpenGLComponent().getRenderer();
	if (renderer != nullptr && isVisible()) {
		renderer->addChangeListener(this);
		renderer->requestRenderedImage();
	}
}

ScopesComponent::~ScopesComponent() {
	removeAsListener();
	histogramComponent = nullptr;
	concertinaPanel = nullptr;
}

void ScopesComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == renderer) {
		// wheee the image is ready to be taken out of the oven :o
		renderer->removeChangeListener(this);
		image = renderer->getRenderedImage();

		const int numPanels = concertinaPanel->getNumPanels();
		for (int i = 0; i < numPanels; i++) {
			concertinaPanel->getPanel(i)->repaint();
		}
	} else {
		// time or data changed, therefore the image was probably rerendered
		if (isVisible()) {
			renderer->addChangeListener(this);
			renderer->requestRenderedImage();
		}
	}
}

void ScopesComponent::resized() {
	concertinaPanel->setBounds(getLocalBounds());
}

void ScopesComponent::visibilityChanged() {
	if (isVisible()) {
		if (renderer != nullptr) {
			renderer->addChangeListener(this);
			renderer->requestRenderedImage();
		}
		addAsListener();
	} else {
		removeAsListener();
	}
}

// ValueTree::Listener callbacks
void ScopesComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
	if (isVisible()) {
		renderer->addChangeListener(this);
		renderer->requestRenderedImage();
	}
}

void ScopesComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
	if (isVisible()) {
		renderer->addChangeListener(this);
		renderer->requestRenderedImage();
	}
}

void ScopesComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
	if (isVisible()) {
		renderer->addChangeListener(this);
		renderer->requestRenderedImage();
	}
}

void ScopesComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void ScopesComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void ScopesComponent::addAsListener() {
	if (renderer != nullptr) {
		renderer->addChangeListener(this);
	}
	data.addListenerToTree(this);
	audioManager.addChangeListener(this);
	if (CameraController::globalCameraController != nullptr) {
		CameraController::globalCameraController->addChangeListener(this);
	}
}

void ScopesComponent::removeAsListener() {
	if (renderer != nullptr) {
		renderer->removeChangeListener(this);
	}
	data.removeListenerFromTree(this);
	audioManager.removeChangeListener(this);
	if (CameraController::globalCameraController != nullptr) {
		CameraController::globalCameraController->removeChangeListener(this);
	}
}


HistogramComponent::HistogramComponent(Image& image_) :
	image(image_)
{
	setName("Histogram");
	setBufferedToImage(true);
}

void HistogramComponent::paint(Graphics& g) {
	g.fillAll(image.getPixelAt(1,1));
}


#include "ScopesComponent.h"

#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <Renderer.h>

ScopesComponent::ScopesComponent() :
	renderer(nullptr)
{
	histogramComponent = new HistogramComponent();

	concertinaPanel = new ConcertinaPanel();
	concertinaPanel->addPanel(0, histogramComponent, false);
	addAndMakeVisible(concertinaPanel);

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
	if (renderer != nullptr) {
		renderer->removeChangeListener(this);
	}
	histogramComponent = nullptr;
	concertinaPanel = nullptr;
}

void ScopesComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	renderer->removeChangeListener(this);
	image = renderer->getRenderedImage();
}

void ScopesComponent::resized() {
	concertinaPanel->setBounds(getLocalBounds());
}

void ScopesComponent::visibilityChanged() {
	if (renderer != nullptr) {
		if (isVisible()) {
			renderer->addChangeListener(this);
			renderer->requestRenderedImage();
		} else {
			renderer->removeChangeListener(this);
		}
	}
}


HistogramComponent::HistogramComponent() {
	setName("Histogram");
	setBufferedToImage(true);
}

void HistogramComponent::paint(Graphics& g) {
	g.fillAll(Colours::red);
}

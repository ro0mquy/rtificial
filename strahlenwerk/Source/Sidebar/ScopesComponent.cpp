#include "ScopesComponent.h"

#include <StrahlenwerkApplication.h>

ScopesComponent::ScopesComponent() {
	postprocPipeline = StrahlenwerkApplication::getInstance()->getProject().getPostproc();

	histogramComponent = new HistogramComponent();

	concertinaPanel = new ConcertinaPanel();
	concertinaPanel->addPanel(0, histogramComponent, false);
	addAndMakeVisible(concertinaPanel);

	if (isVisible()) {
		//postprocPipeline->addChangeListener(this);
		//postprocPipeline->requestRenderedImage();
	}
}

ScopesComponent::~ScopesComponent() {
	//postprocPipeline->removeChangeListener(this);
	histogramComponent = nullptr;
	concertinaPanel = nullptr;
}

void ScopesComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	postprocPipeline->removeChangeListener(this);
	//image = postprocPipeline->getRenderedImage();
}

void ScopesComponent::resized() {
	concertinaPanel->setBounds(getLocalBounds());
}

void ScopesComponent::visibilityChanged() {
	if (isVisible()) {
	} else {
		//postprocPipeline->removeChangeListener(this);
	}
}


HistogramComponent::HistogramComponent() {
	setName("Histogram");
	setBufferedToImage(true);
}

void HistogramComponent::paint(Graphics& g) {
	g.fillAll(Colours::red);
}

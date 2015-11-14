#include "ScopesComponent.h"

#include <algorithm>

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
	vectorscopeComponent = new VectorscopeComponent(image);

	concertinaPanel = new ConcertinaPanel();
	concertinaPanel->addPanel(0, histogramComponent, false);
	concertinaPanel->addPanel(1, vectorscopeComponent, false);
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

	mode = LumaMode;
	mode.addListener(this);
	PropertyComponent* modeSelector = new ChoicePropertyComponent(
		mode,
		"Display Mode",
		{ "Luma", "Red", "Green", "Blue" },
		{ LumaMode, RMode, GMode, BMode }
	);

	logScale = false;
	logScale.addListener(this);
	PropertyComponent* logScaleSelector = new BooleanPropertyComponent(logScale, "Log Scale", String::empty);
	logScaleSelector->setColour(BooleanPropertyComponent::backgroundColourId, Colour(0x00000000));

	propertyPanel.addProperties({{modeSelector, logScaleSelector}});

	resized();
	addAndMakeVisible(propertyPanel);
}

void HistogramComponent::valueChanged(Value& /*value*/) {
	repaint();
}

void HistogramComponent::resized() {
	const Rectangle<int> boundsRect = getLocalBounds().reduced(padding);
	Rectangle<int> propertyBounds(boundsRect);
	propertyBounds.setHeight(jmin(boundsRect.getHeight(), propertyPanel.getTotalContentHeight()));
	propertyBounds.setWidth(jmin(boundsRect.getWidth(), 150));
	propertyBounds.setY(jmax(padding, boundsRect.getHeight()-padding));
	propertyPanel.setBounds(propertyBounds);
}

void HistogramComponent::paint(Graphics& g) {
	const int gradBarHeight = 10;
	const int gradBarPadding = 10;
	const Rectangle<int> plotCanvas(
		padding,
		padding,
		g.getClipBounds().getWidth() - 2*padding,
		g.getClipBounds().getHeight() - 2*padding - propertyPanel.getBounds().getHeight() - padding - gradBarHeight - gradBarPadding
	);
	const Rectangle<int> gradBar(
		plotCanvas.getX(),
		plotCanvas.getY() + plotCanvas.getHeight() + gradBarPadding,
		plotCanvas.getWidth(),
		gradBarHeight
	);

	if (plotCanvas.isEmpty()) {
		return;
	}

	// use floats for effective log-scale, init with 0s
	float values[255] = {};

	const int w = image.getWidth();
	const int h = image.getHeight();
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			const Colour color = image.getPixelAt(x, y);
			uint8 value;

			if (mode == RMode) {
				value = uint8(color.getRed() * 255);
			} else if (mode == GMode) {
				value = uint8(color.getGreen() * 255);
			} else if (mode == BMode) {
				value = uint8(color.getBlue() * 255);
			} else {
				value = uint8(color.getBrightness() * 255);
			}

			values[value] += 1;
		}
	}

	if (logScale == true) {
		for (int i = 0; i < 255; i++) {
			if (values[i] > 0) {
				values[i] = log(values[i]);
			}
		}
	}

	g.setColour(findColour(HistogramComponent::backgroundColourId));
	g.fillRect(plotCanvas);

	g.setColour(findColour(HistogramComponent::rulerColourId));
	for (int i = 1; i <= 3; i++) {
		const float x = plotCanvas.getX() + i*1.0f/4.0f * plotCanvas.getWidth();
		g.drawLine(x, plotCanvas.getY(), x, plotCanvas.getY() + plotCanvas.getHeight(), 2.0f);
	}

	Colour color;
	if (mode == RMode) {
		color = findColour(HistogramComponent::redColourId);
	} else if (mode == GMode) {
		color = findColour(HistogramComponent::greenColourId);
	} else if (mode == BMode) {
		color = findColour(HistogramComponent::blueColourId);
	} else {
		color = findColour(HistogramComponent::lumaColourId);
	}
	g.setColour(color);
	const float barWidth = plotCanvas.getWidth()/255.0f;
	const float maxValue = *std::max_element(values, values+255);
	const float scaleY = ((maxValue == 0) ? 0 : plotCanvas.getHeight()/maxValue);
	for (int i = 0; i < 255; i++) {
		g.fillRect(
			float(plotCanvas.getX() + (i * barWidth)),
			float(plotCanvas.getY() + plotCanvas.getHeight() - (values[i] * scaleY)),
			float(barWidth),
			float(values[i] * scaleY)
		);
	}

	ColourGradient gradient = ColourGradient(Colour(0xff000000), gradBar.getX(), gradBar.getY(), color, gradBar.getWidth(), gradBar.getY(), false);
	g.setGradientFill(gradient);
	g.fillRect(gradBar);
}


VectorscopeComponent::VectorscopeComponent(Image& image_) :
	image(image_)
{
	setName("Vectorscope");
	setBufferedToImage(true);

	Label* label = new Label("Label", "not implemented.");
	label->setSize(100,20);
	addAndMakeVisible(label);
}

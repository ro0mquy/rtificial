#include "SequenceViewComponent.h"

#include <cmath>
#include "SequenceComponent.h"
#include "TimelineData.h"
#include "KeyframeComponent.h"

SequenceViewComponent::SequenceViewComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
	zoomFactor.addListener(this);
	updateSequenceComponents();
}

SequenceViewComponent::~SequenceViewComponent() = default;

void SequenceViewComponent::updateSize() {
	const int paddingAfterLastScene = 300;
	const int rowHeight = 20;
	const int numUniforms = data.getNumUniforms();
	const int endTime = data.getLastSceneEndTime() * zoomFactor;

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int width = jmax(endTime + paddingAfterLastScene, viewportWidth);
	const int height = jmax(numUniforms * rowHeight, viewportHeight);
	setSize(width, height);
}

void SequenceViewComponent::paint(Graphics& g){
	const int rowHeight = 20;

	const int numUniforms = data.getNumUniforms();
	const int numScenes = data.getNumScenes();
	for(int i = 0; i < numUniforms; i++){
		// draw rows
		ValueTree uniform = data.getUniform(i);
		const Rectangle<float> rectRow(0, i*rowHeight, getWidth(), rowHeight);
		g.setColour(findColour(i%2 == 0 ? SequenceViewComponent::evenRowColourId : SequenceViewComponent::oddRowColourId));
		g.fillRect(rectRow);
		g.setColour(findColour(SequenceViewComponent::seperatorColourId));
		g.drawHorizontalLine((i + 1) * rowHeight - 1, 0, getWidth());

		// mark inactive areas
		for (int j = 0; j < numScenes; j++) {
			ValueTree scene = data.getScene(i);
			if (!uniformActiveForScene(uniform, scene)) {
				const float start = (float) data.getSceneStart(scene) * zoomFactor;
				const float duration = (float) data.getSceneDuration(scene) * zoomFactor;
				const Rectangle<float> rectInactiveArea(start, i*rowHeight, duration, rowHeight);
				g.setColour(findColour(SequenceViewComponent::inactiveAreaColourId));
				g.fillRect(rectInactiveArea);
			}
		}
	}
}

void SequenceViewComponent::paintOverChildren(Graphics& g) {
	// draw time marker
	g.setColour(findColour(SequenceViewComponent::timeMarkerColourId));
	const float x = (float) data.currentTime.getValue() * zoomFactor;
	g.drawLine(x, 0, x, getHeight(), 2);
}

bool SequenceViewComponent::uniformActiveForScene(ValueTree uniform, ValueTree scene) {
	// TODO: dummy functionality, replace with real lookup
	const String uniformName = data.getUniformName(uniform);
	const String sceneShaderSource = data.getSceneShaderSource(scene);
	const int64 hash = uniformName.hashCode() + sceneShaderSource.hashCode();
	return hash % 2 != 0;
}

void SequenceViewComponent::updateSequenceComponents() {
	const int rowHeight = 20;
	sequenceComponentsArray.clearQuick(true);
	const int numUniforms = data.getNumUniforms();

	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = data.getUniform(i);
		const int numSequences = data.getNumSequences(uniform);

		for (int j = 0; j < numSequences; j++) {
			ValueTree sequenceData = data.getSequence(uniform, j);
			auto sequenceComponent = new SequenceComponent(sequenceData, i*rowHeight, rowHeight);
			addAndMakeVisible(sequenceComponent);
			sequenceComponentsArray.add(sequenceComponent);
		}
	}
}

void SequenceViewComponent::removeSequenceComponent(const SequenceComponent* toBeDeleted) {
	sequenceComponentsArray.removeObject(toBeDeleted);
}

void SequenceViewComponent::mouseDown(const MouseEvent& event) {
	const int rowHeight = 20;
	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
	ValueTree uniform = data.getUniform(numUniform);
	if (!uniform.isValid()) {
		// click in empty area
		return;
	}

	const int absoluteStart = event.getMouseDownX() / zoomFactor;
	var sequenceDuration = 0;
	var sequenceInterpolation = "linear";
	newSequenceData = data.addSequence(uniform, absoluteStart, sequenceDuration, sequenceInterpolation);

	newSequenceComponent = new SequenceComponent(newSequenceData, numUniform * rowHeight, rowHeight);
	addAndMakeVisible(newSequenceComponent);
}

void SequenceViewComponent::mouseDrag(const MouseEvent& event) {
	if (!newSequenceData.isValid()) {
		// click in empty area
		return;
	}

	const int gridWidth = 20;

	const float mouseDown = event.getMouseDownX() / zoomFactor;
	const int mouseDownGrid = roundFloatToInt(mouseDown / float(gridWidth)) * gridWidth;

	const float mousePos = event.position.getX() / zoomFactor;
	const int mousePosGrid = roundFloatToInt(mousePos / float(gridWidth)) * gridWidth;

	const int distanceGrid = mousePosGrid - mouseDownGrid;
	const int absDistanceGrid = abs(distanceGrid);

	const int absoluteStartGrid = mouseDownGrid + jmin(0, distanceGrid); // subtract distance if negative

	data.setSequencePropertiesForAbsoluteStart(newSequenceData, absoluteStartGrid);
	data.setSequenceDuration(newSequenceData, absDistanceGrid);
	newSequenceComponent->updateSceneStartValueRefer();
	newSequenceComponent->updateBounds();
}

void SequenceViewComponent::mouseUp(const MouseEvent& /*event*/) {
	if (!newSequenceData.isValid()) {
		// click in empty area
		return;
	}

	if (int(data.getSequenceDuration(newSequenceData)) == 0) {
		newSequenceComponent = nullptr; // this deletes the component
		data.removeSequence(newSequenceData);
	} else {
		sequenceComponentsArray.add(newSequenceComponent.release()); // release() sets the pointer to nullptr
	}
	newSequenceData = ValueTree();
}

void SequenceViewComponent::zoomFactorChanged(ZoomFactor&) {
	updateSize();
	repaint();
}

#include "SequenceViewComponent.h"

#include <cmath>
#include "TreeIdentifiers.h"
#include "SequenceComponent.h"
#include "TimelineData.h"
#include "KeyframeComponent.h"

SequenceViewComponent::SequenceViewComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
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
	const float x = data.currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);
}

bool SequenceViewComponent::uniformActiveForScene(ValueTree uniform, ValueTree scene) {
	// dummy functionality, replace with real lookup
	const String uniformName = uniform.getProperty(treeId::uniformName);
	const String sceneShaderSource = scene.getProperty(treeId::sceneShaderSource);
	const int64 hash = uniformName.hashCode() + sceneShaderSource.hashCode();
	return hash % 2 != 0;
}

void SequenceViewComponent::updateSequenceComponents() {
	const int rowHeight = 20;
	sequenceComponentsArray.clearQuick(true);
	ValueTree uniformsArray = data.getUniformsArray();
	const int numUniforms = uniformsArray.getNumChildren();

	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = uniformsArray.getChild(i);
		ValueTree sequencesArray = data.getSequencesArray(uniform);
		const int numSequences = sequencesArray.getNumChildren();

		for (int j = 0; j < numSequences; j++) {
			ValueTree sequenceData = sequencesArray.getChild(j);
			SequenceComponent* sequenceComponent = new SequenceComponent(sequenceData, i*rowHeight, rowHeight);
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
	if (numUniform >= data.getNumUniforms()) {
		// click in empty area
		return;
	}

	const int absoluteStart = event.getMouseDownX();
	ValueTree scene = data.getSceneForTime(absoluteStart);
	const int relativeStart = absoluteStart - int(scene.getProperty(treeId::sceneStart));

	newSequenceData = ValueTree(treeId::sequence);
	newSequenceData.setProperty(treeId::sequenceSceneId, scene.getProperty(treeId::sceneId), nullptr);
	newSequenceData.setProperty(treeId::sequenceStart, var(relativeStart), nullptr);
	newSequenceData.setProperty(treeId::sequenceDuration, var(0), nullptr);
	newSequenceData.setProperty(treeId::sequenceInterpolation, var("linear"), nullptr);

	ValueTree uniform = data.getUniformsArray().getChild(numUniform);
	data.addSequence(uniform, newSequenceData);

	newSequenceComponent = new SequenceComponent(newSequenceData, numUniform * rowHeight, rowHeight);
	addAndMakeVisible(newSequenceComponent);
}

void SequenceViewComponent::mouseDrag(const MouseEvent& event) {
	if (!newSequenceData.isValid()) {
		// click in empty area
		return;
	}

	const int gridWidth = 20;

	const int mouseDown = event.getMouseDownX();
	const int mouseDownGrid = roundFloatToInt(float(mouseDown) / float(gridWidth)) * gridWidth;

	const int mousePos = event.getPosition().getX();
	const int mousePosGrid = roundFloatToInt(float(mousePos) / float(gridWidth)) * gridWidth;

	const int distanceGrid = mousePosGrid - mouseDownGrid;
	const int absDistanceGrid = abs(distanceGrid);

	const int absoluteStartGrid = mouseDownGrid + jmin(0, distanceGrid); // subtract distance if negative
	ValueTree scene = data.getSceneForTime(absoluteStartGrid);
	const int relativeStartGrid = absoluteStartGrid - int(scene.getProperty(treeId::sceneStart));

	newSequenceData.setProperty(treeId::sequenceSceneId, scene.getProperty(treeId::sceneId), nullptr);
	newSequenceData.setProperty(treeId::sequenceStart, var(relativeStartGrid), nullptr);
	newSequenceData.setProperty(treeId::sequenceDuration, var(absDistanceGrid), nullptr);
	newSequenceComponent->updateSceneStartValueRefer();
	newSequenceComponent->updateBounds();
}

void SequenceViewComponent::mouseUp(const MouseEvent& event) {
	if (!newSequenceData.isValid()) {
		// click in empty area
		return;
	}

	if (int(newSequenceData.getProperty(treeId::sequenceDuration)) == 0) {
		newSequenceComponent = nullptr;
		newSequenceData.getParent().removeChild(newSequenceData, nullptr);
	} else {
		sequenceComponentsArray.add(newSequenceComponent.release());
	}
	newSequenceData = ValueTree();
}

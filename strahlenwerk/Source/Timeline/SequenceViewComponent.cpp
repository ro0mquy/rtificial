#include "SequenceViewComponent.h"

#include "TreeIdentifiers.h"
#include "SequenceComponent.h"
#include "Data.h"
#include "../StrahlenwerkApplication.h"

SequenceViewComponent::SequenceViewComponent() :
	data(StrahlenwerkApplication::getInstance()->getTimelineData())
{
	updateSequenceComponents();
}

SequenceViewComponent::~SequenceViewComponent() = default;

void SequenceViewComponent::updateSize() {
	const int width = jmax(data.getLastSceneEndTime() + 20, getParentWidth());
	const int height = jmax(data.getUniformsArray().getNumChildren() * 20, getParentHeight());
	setSize(width, height);
}

void SequenceViewComponent::paint(Graphics& g){
	const int rowHeight = 20;

	ValueTree uniformsArray = data.getUniformsArray();
	const int numUniforms = uniformsArray.getNumChildren();

	ValueTree scenesArray = data.getScenesArray();
	const int numScenes = scenesArray.getNumChildren();

	for(int i = 0; i < numUniforms; i++){
		// draw rows
		ValueTree uniform = uniformsArray.getChild(i);
		const Rectangle<float> rect(0, i*rowHeight, getWidth(), rowHeight);
		g.setColour(findColour(i%2 == 0 ? SequenceViewComponent::evenRowColourId : SequenceViewComponent::oddRowColourId));
		g.fillRect(rect);
		g.setColour(findColour(SequenceViewComponent::seperatorColourId));
		g.drawHorizontalLine(i*rowHeight+rowHeight-1, 0, getWidth());

		// mark inactive areas
		for (int j = 0; j < numScenes; j++) {
			ValueTree scene = scenesArray.getChild(j);
			if (!uniformActiveForScene(uniform, scene)) {
				const int start = scene.getProperty(treeId::sceneStart);
				const int duration = scene.getProperty(treeId::sceneDuration);
				const Rectangle<float> rectangle(start, i*rowHeight, duration, rowHeight);
				g.setColour(findColour(SequenceViewComponent::inactiveAreaColourId));
				g.fillRect(rectangle);
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

void SequenceViewComponent::mouseDown(const MouseEvent& event) {
	const int absoluteStart = event.getMouseDownX();
	ValueTree scene = data.getSceneForTime(absoluteStart);
	const int relativeStart = absoluteStart - int(scene.getProperty(treeId::sceneStart));

	newSequenceData = ValueTree(treeId::sequence);
	newSequenceData.setProperty(treeId::sequenceSceneId, scene.getProperty(treeId::sceneId), nullptr);
	newSequenceData.setProperty(treeId::sequenceStart, var(relativeStart), nullptr);
	newSequenceData.setProperty(treeId::sequenceDuration, var(0), nullptr);
	newSequenceData.setProperty(treeId::sequenceInterpolation, var("linear"), nullptr);

	const int rowHeight = 20;
	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
	newSequenceComponent = new SequenceComponent(newSequenceData, numUniform * rowHeight, rowHeight);
	addAndMakeVisible(newSequenceComponent);
}

void SequenceViewComponent::mouseDrag(const MouseEvent& event) {
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
	if (int(newSequenceData.getProperty(treeId::sequenceDuration)) == 0) {
		delete newSequenceComponent;
	} else {
		sequenceComponentsArray.add(newSequenceComponent);

		const int rowHeight = 20;
		const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
		ValueTree uniform = data.getUniformsArray().getChild(numUniform);
		data.addSequence(uniform, newSequenceData);
	}
	newSequenceComponent = nullptr;
	newSequenceData = ValueTree();
}

#include "SequenceViewComponent.h"
#include "TreeIdentifiers.h"

SequenceViewComponent::SequenceViewComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
	updateSequenceComponents();
}

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

	// draw time marker
	g.setColour(findColour(SequenceViewComponent::timeMarkerColourId));
	const float x = currentTime.getValue();
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
			SequenceComponent* sequenceComponent = new SequenceComponent(sequenceData, data, i*rowHeight, rowHeight);
			addAndMakeVisible(sequenceComponent);
			sequenceComponentsArray.add(sequenceComponent);
		}
	}
}

#include "SequenceViewComponent.h"

#include <cmath>
#include "SequenceComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "KeyframeComponent.h"

SequenceViewComponent::SequenceViewComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_)
{
	data.addListenerToTree(this);
	zoomFactor.addListener(this);
	data.currentTime.addListener(this);
	addAllSequenceComponents();
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

void SequenceViewComponent::addSequenceComponent(ValueTree sequenceData) {
	auto sequenceComponent = new SequenceComponent(sequenceData, zoomFactor);
	addAndMakeVisible(sequenceComponent);
	sequenceComponentsArray.add(sequenceComponent);
}

void SequenceViewComponent::addAllSequenceComponents() {
	sequenceComponentsArray.clearQuick(true);
	const int numUniforms = data.getNumUniforms();

	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = data.getUniform(i);
		const int numSequences = data.getNumSequences(uniform);

		for (int j = 0; j < numSequences; j++) {
			ValueTree sequenceData = data.getSequence(uniform, j);
			addSequenceComponent(sequenceData);
		}
	}
}

SequenceComponent* SequenceViewComponent::getSequenceComponentForData(ValueTree sequenceData) {
	const int componentsArraySize = sequenceComponentsArray.size();
	for (int i = 0; i < componentsArraySize; i++) {
		auto sequenceComponent = sequenceComponentsArray.getUnchecked(i);
		if (sequenceComponent->sequenceData == sequenceData) {
			return sequenceComponent;
		}
	}
	return nullptr;
}

void SequenceViewComponent::mouseDown(const MouseEvent& event) {
	const int gridWidth = 20;
	const int rowHeight = 20;
	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
	ValueTree uniform = data.getUniform(numUniform);

	// uniform is invalid if click was in empty area
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown() && uniform.isValid()) {
		const float absoluteStart = event.getMouseDownX() / zoomFactor;
		const int absoluteStartGrid = roundFloatToInt(absoluteStart / float(gridWidth)) * gridWidth;
		var sequenceDuration = 0;
		var sequenceInterpolation = "linear";
		currentlyCreatedSequenceData = data.addSequence(uniform, absoluteStartGrid, sequenceDuration, sequenceInterpolation);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SequenceViewComponent::mouseDrag(const MouseEvent& event) {
	// invalid data happens on click in empty area, no left click or no command down
	if (currentlyCreatedSequenceData.isValid()) {
		const int gridWidth = 20;

		const float mouseDown = event.getMouseDownX() / zoomFactor;
		const int mouseDownGrid = roundFloatToInt(mouseDown / float(gridWidth)) * gridWidth;

		const float mousePos = event.position.getX() / zoomFactor;
		const int mousePosGrid = roundFloatToInt(mousePos / float(gridWidth)) * gridWidth;

		const int distanceGrid = mousePosGrid - mouseDownGrid;
		const int absDistanceGrid = abs(distanceGrid);

		const int absoluteStartGrid = mouseDownGrid + jmin(0, distanceGrid); // subtract distance if negative

		data.setSequencePropertiesForAbsoluteStart(currentlyCreatedSequenceData, absoluteStartGrid);
		data.setSequenceDuration(currentlyCreatedSequenceData, absDistanceGrid);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void SequenceViewComponent::mouseUp(const MouseEvent& event) {
	// invalid data happens on click in empty area, no left click or no command down
	if (currentlyCreatedSequenceData.isValid()) {
		if (int(data.getSequenceDuration(currentlyCreatedSequenceData)) == 0) {
			data.removeSequence(currentlyCreatedSequenceData);
		}
		currentlyCreatedSequenceData = ValueTree();
	} else {
		McbComponent::mouseUp(event);
	}
}

void SequenceViewComponent::zoomFactorChanged(ZoomFactor&) {
	updateSize();
	repaint();
}

void SequenceViewComponent::valueChanged(Value& /*value*/) {
	// currentTime changed
	repaint();
}

// ValueTree::Listener callbacks
void SequenceViewComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& /*property*/) {
	if (parentTree.hasType(treeId::scene)) {
		updateSize();
		repaint();
	}
}

void SequenceViewComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (data.isSequence(childWhichHasBeenAdded)) {
		addSequenceComponent(childWhichHasBeenAdded);
	} else if (childWhichHasBeenAdded.hasType(treeId::scene)) {
		updateSize();
		repaint();
	} else if (childWhichHasBeenAdded.hasType(treeId::uniform)) {
		updateSize();
		repaint();
	}
}

void SequenceViewComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved) {
	if (data.isSequence(childWhichHasBeenRemoved)) {
		auto sequenceComponent = getSequenceComponentForData(childWhichHasBeenRemoved);
		jassert(sequenceComponent != nullptr);
		sequenceComponentsArray.removeObject(sequenceComponent);
	} else if (childWhichHasBeenRemoved.hasType(treeId::scene)) {
		updateSize();
		repaint();
	} else if (childWhichHasBeenRemoved.hasType(treeId::uniform)) {
		updateSize();
		repaint();
	}
}

void SequenceViewComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void SequenceViewComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void SequenceViewComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always the root tree
	updateSize();
	repaint();
	addAllSequenceComponents();
}

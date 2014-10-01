#include "SequenceViewComponent.h"

#include <cmath>
#include "SequenceComponent.h"
#include "KeyframeComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "ZoomFactor.h"

SequenceViewComponent::SequenceViewComponent(ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	timeMarker(zoomFactor_)
{
	data.addListenerToTree(this);
	zoomFactor.addChangeListener(this);
	addAllSequenceComponents();
	addAndMakeVisible(timeMarker);
}

SequenceViewComponent::~SequenceViewComponent() {
	data.removeListenerFromTree(this);
	zoomFactor.removeChangeListener(this);
}

void SequenceViewComponent::updateSize() {
	const int rowHeight = 20;
	const int numUniforms = data.getNumUniforms();
	const float paddingAfterLastScene = 300.;
	const float endTime = data.getLastSceneEndTime() * zoomFactor;

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int width = jmax(roundFloatToInt(endTime + paddingAfterLastScene), viewportWidth);
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
	const int rowHeight = 20;
	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
	ValueTree uniform = data.getUniform(numUniform);

	// uniform is invalid if click was in empty area
	const ModifierKeys& m = event.mods;
	if (m.isLeftButtonDown() && m.isCommandDown() && uniform.isValid()) {
		const float absoluteStart = event.getMouseDownX() / zoomFactor;
		const float absoluteStartGrid = zoomFactor.snapValueToGrid(absoluteStart);
		var sequenceDuration = 0.f;
		var sequenceInterpolation = "linear";
		currentlyCreatedSequenceData = data.addSequence(uniform, absoluteStartGrid, sequenceDuration, sequenceInterpolation);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SequenceViewComponent::mouseDrag(const MouseEvent& event) {
	// invalid data happens on click in empty area, no left click or no command down
	if (currentlyCreatedSequenceData.isValid()) {
		const float mouseDown = event.getMouseDownX() / zoomFactor;
		const float mouseDownGrid = zoomFactor.snapValueToGrid(mouseDown);

		const float mousePos = event.position.getX() / zoomFactor;
		const float mousePosGrid = zoomFactor.snapValueToGrid(mousePos);

		const float distanceGrid = mousePosGrid - mouseDownGrid;
		const float absDistanceGrid = std::abs(distanceGrid);

		const float absoluteStartGrid = mouseDownGrid + jmin(0.f, distanceGrid); // subtract distance if negative

		data.setSequencePropertiesForAbsoluteStart(currentlyCreatedSequenceData, absoluteStartGrid);
		data.setSequenceDuration(currentlyCreatedSequenceData, absDistanceGrid);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void SequenceViewComponent::mouseUp(const MouseEvent& event) {
	// invalid data happens on click in empty area, no left click or no command down
	if (currentlyCreatedSequenceData.isValid()) {
		if (0.f == float(data.getSequenceDuration(currentlyCreatedSequenceData))) {
			data.removeSequence(currentlyCreatedSequenceData);
		}
		currentlyCreatedSequenceData = ValueTree();
	} else {
		McbComponent::mouseUp(event);
	}
}

void SequenceViewComponent::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// zoomFactor changed
	updateSize();
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
		timeMarker.updateSize();
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
		timeMarker.updateSize();
	}
}

void SequenceViewComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void SequenceViewComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void SequenceViewComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always the root tree
	addAllSequenceComponents();
	updateSize();
	repaint();
}

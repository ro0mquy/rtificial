#include "SequenceViewComponent.h"

#include <cmath>
#include "SequenceComponent.h"
#include "KeyframeComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "SectionManager.h"
#include "ZoomFactor.h"
#include <RtificialLookAndFeel.h>

SequenceViewComponent::SequenceViewComponent(SectionManager& sectionManager_, ZoomFactor& zoomFactor_) :
	data(TimelineData::getTimelineData()),
	sectionManager(sectionManager_),
	zoomFactor(zoomFactor_),
	timeMarker(zoomFactor_)
{
	data.addListenerToTree(this);
	sectionManager.addListenerToTree(this);
	zoomFactor.addChangeListener(this);
	addAllSequenceComponents();
	addAndMakeVisible(timeMarker);
}

SequenceViewComponent::~SequenceViewComponent() {
	data.removeListenerFromTree(this);
	sectionManager.removeListenerFromTree(this);
	zoomFactor.removeChangeListener(this);
}

void SequenceViewComponent::updateSize() {
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	const int totalRows = sectionManager.getTotalHeightInRows();
	const int paddingAfterLastScene = 300;
	const int endTime = roundFloatToInt(data.getLastSceneEndTime() * zoomFactor);

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int width = jmax(endTime + paddingAfterLastScene, viewportWidth);
	const int height = jmax(totalRows * rowHeight, viewportHeight);
	setSize(width, height);

	timeMarker.updateSize();
}

void SequenceViewComponent::paint(Graphics& g){
	/*
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
	// */

	// draw stripes
	const int myWidth = getWidth();
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	const int totalRows = sectionManager.getTotalHeightInRows();
	for (int i = 0; i < totalRows; i++) {
		g.setColour(findColour(i%2 == 0 ? SequenceViewComponent::evenRowColourId : SequenceViewComponent::oddRowColourId));
		g.fillRect(0, i*rowHeight, myWidth, rowHeight);

		g.setColour(findColour(SequenceViewComponent::uniformSeperatorColourId));
		g.drawHorizontalLine((i + 1) * rowHeight - 1, 0, myWidth);
	}

	SectionTypes::Section& rootSection = sectionManager.getRootSection();
	Rectangle<int> targetBounds = getLocalBounds();
	drawSectionHeader(g, rootSection, targetBounds);
}

void SequenceViewComponent::drawSectionHeader(Graphics& g, SectionTypes::Section& section, Rectangle<int>& targetBounds) const {
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;

	// draw section header
	const String sectionName = sectionManager.getSectionName(section);
	if (sectionName.isNotEmpty()) {
		// don't draw root section header

		const Colour sectionColor = RtificialLookAndFeel::getColorFromSectionName(sectionName);

		const Rectangle<int> headerRect = targetBounds.removeFromTop(rowHeight);
		//g.setColour(findColour(SequenceViewComponent::sectionHeaderSeperatorColourId));
		//g.drawHorizontalLine(headerRect.getCentreY(), headerRect.getX(), headerRect.getRight());
		g.setColour(sectionColor.withAlpha(0.35f));
		const int sectionLineHeight = 4;
		const Rectangle<int> sectionLine(
			headerRect.getX(),
			headerRect.getY() + ( (headerRect.getHeight() - sectionLineHeight) / 2 ),
			headerRect.getWidth(),
			sectionLineHeight
		);
		g.drawLine(headerRect.getX(), headerRect.getCentreY(), headerRect.getRight(), headerRect.getCentreY(), 4.0f);
	}

	if (sectionManager.getSectionCollapsed(section)) {
		// the section is collapsed
		return;
	}

	// recursivly draw subsections
	const int numSections = sectionManager.getNumSections(section);
	for (int i = 0; i < numSections; i++) {
		SectionTypes::Section subsection = sectionManager.getSection(section, i);
		drawSectionHeader(g, subsection, targetBounds);
	}

	// leave uniform space empty
	const int numUniforms = sectionManager.getNumUniforms(section);
	targetBounds.removeFromTop(numUniforms * rowHeight);
}

bool SequenceViewComponent::uniformActiveForScene(ValueTree /*uniform*/, ValueTree /*scene*/) {
	return true;
	/*
	// TODO: dummy functionality, replace with real lookup
	const String uniformName = data.getUniformName(uniform);
	const String sceneShaderSource = data.getSceneShaderSource(scene);
	const int64 hash = uniformName.hashCode() + sceneShaderSource.hashCode();
	return hash % 2 != 0;
	*/
}

void SequenceViewComponent::addSequenceComponent(ValueTree sequenceData) {
	const int uniformRow = sectionManager.getUniformYPosInRows(data.getUniformName(data.getSequenceParentUniform(sequenceData)));
	addSequenceComponent(sequenceData, uniformRow);
}

void SequenceViewComponent::addSequenceComponent(ValueTree sequenceData, const int uniformRow) {
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	SequenceComponent* sequenceComponent = new SequenceComponent(sequenceData, zoomFactor, sectionManager);
	sequenceComponent->setBounds(sequenceComponent->getX(), uniformRow * rowHeight, sequenceComponent->getWidth(), rowHeight); // SequenceComponents set their x dimensions by themselves
	addAndMakeVisible(sequenceComponent);
	sequenceComponentsArray.add(sequenceComponent);
}

void SequenceViewComponent::addAllSequenceComponents() {
	sequenceComponentsArray.clearQuick(true);

	const int numUniforms = data.getNumUniforms();
	for (int i = 0; i < numUniforms; i++) {
		ValueTree uniform = data.getUniform(i);
		const int uniformRow = sectionManager.getUniformYPosInRows(data.getUniformName(uniform));

		if (uniformRow == -1) {
			// uniform is not visible
			continue;
		}

		const int numSequences = data.getNumSequences(uniform);
		for (int j = 0; j < numSequences; j++) {
			ValueTree sequenceData = data.getSequence(uniform, j);
			addSequenceComponent(sequenceData, uniformRow);
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
	const int numRow = int(float(event.getMouseDownY()) / float(rowHeight));
	SectionTypes::Uniform uniformSectionTree = sectionManager.getUniformOrSectionForYPos(numRow);

	// uniform is invalid if click was on section header or in empty area
	const ModifierKeys& m = event.mods;
	if (sectionManager.isUniform(uniformSectionTree) && m == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		ValueTree uniformData = sectionManager.getTimelineUniformForSectionUniform(uniformSectionTree);
		jassert(uniformData.isValid());
		const int absoluteStart = event.getMouseDownX() / zoomFactor;
		const int absoluteStartGrid = zoomFactor.snapValueToGrid(absoluteStart);
		var sequenceDuration = 0.f;
		var sequenceInterpolation = "linear";
		data.getUndoManager().beginNewTransaction("Create Sequence");
		currentlyCreatedSequenceData = data.addSequence(uniformData, absoluteStartGrid, sequenceDuration, sequenceInterpolation);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SequenceViewComponent::mouseDrag(const MouseEvent& event) {
	// invalid data happens on click in empty area, no left click or no command down
	if (currentlyCreatedSequenceData.isValid()) {
		const int mouseDown = event.getMouseDownX() / zoomFactor;
		const int mouseDownGrid = zoomFactor.snapValueToGrid(mouseDown);

		const int mousePos = event.position.getX() / zoomFactor;
		const int mousePosGrid = jmax(0, zoomFactor.snapValueToGrid(mousePos));

		const int distanceGrid = mousePosGrid - mouseDownGrid;
		const int absDistanceGrid = std::abs(distanceGrid);

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
		if (0 == int(data.getSequenceDuration(currentlyCreatedSequenceData))) {
			data.removeSequence(currentlyCreatedSequenceData);
			data.getUndoManager().undoCurrentTransactionOnly();
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
void SequenceViewComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree.hasType(treeId::scene)) {
		updateSize();
		repaint();
	} else if (property == sectionTreeId::sectionCollapsed) {
		// section collapsed state toggled
		addAllSequenceComponents();
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
	} else if (sectionManager.isSection(childWhichHasBeenAdded) || sectionManager.isUniform(childWhichHasBeenAdded)) {
		// section or uniform added
		addAllSequenceComponents();
		updateSize();
		repaint();
	}
}

void SequenceViewComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved, int /*indexFromWhichChildWasRemoved*/) {
	if (data.isSequence(childWhichHasBeenRemoved)) {
		SequenceComponent* sequenceComponent = getSequenceComponentForData(childWhichHasBeenRemoved);
		jassert(sequenceComponent != nullptr);
		sequenceComponentsArray.removeObject(sequenceComponent);
	} else if (childWhichHasBeenRemoved.hasType(treeId::scene)) {
		updateSize();
		repaint();
	} else if (sectionManager.isSection(childWhichHasBeenRemoved) || sectionManager.isUniform(childWhichHasBeenRemoved)) {
		// section or uniform removed
		addAllSequenceComponents();
		updateSize();
		repaint();
	}
}

void SequenceViewComponent::valueTreeChildOrderChanged(ValueTree& parentTree, int /*oldIndex*/, int /*newIndex*/) {
	if (parentTree.hasType(sectionTreeId::sectionsArray) || parentTree.hasType(sectionTreeId::uniformsArray)) {
		// the ordering of some sections or uniforms has changed
		addAllSequenceComponents();
		repaint();
	}
}

void SequenceViewComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void SequenceViewComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always one of the root trees (TimelineData or SectionManager)
	addAllSequenceComponents();
	updateSize();
	repaint();
}

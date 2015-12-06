#include "UniformsBarComponent.h"

#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include <Sidebar/ValueEditorPropertyComponent.h>
#include <RtificialLookAndFeel.h>
#include <Timeline/SectionManager.h>

UniformsBarComponent::UniformsBarComponent(SectionManager& sectionManager_) :
	data(TimelineData::getTimelineData()),
	sectionManager(sectionManager_)
{
	data.addListenerToTree(this);
}

UniformsBarComponent::~UniformsBarComponent() {
	data.removeListenerFromTree(this);
}

void UniformsBarComponent::updateSize() {
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	const int totalRows = sectionManager.getTotalHeightInRows();

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	// we add 18 to compensate the scrollbars that can show up in the sequenceView
	// TODO: find a way to dynamically find the scrollbar height
	const int height = jmax(totalRows * rowHeight + 18, viewportHeight);
	setSize(viewportWidth, height);
}

void UniformsBarComponent::paint(Graphics& g) {
	/*
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	const int numUniforms = data.getNumUniforms();

	for(int i = 0; i < numUniforms; i++) {
		ValueTree uniform = data.getUniform(i);

		const Rectangle<float> rect(0, i*rowHeight, getWidth(), rowHeight);

		g.setColour(findColour(i%2 == 0 ? UniformsBarComponent::evenRowColourId : UniformsBarComponent::oddRowColourId));
		g.fillRect(rect);

		g.setColour(findColour(UniformsBarComponent::seperatorColourId));
		g.drawHorizontalLine(i*rowHeight+rowHeight-1, 0, getWidth());

		const String name = data.getUniformName(uniform);
		g.setColour(findColour(UniformsBarComponent::uniformTextColourId));
		g.drawFittedText(name, rect.withLeft(3).getSmallestIntegerContainer(), Justification::centredLeft, 1);
	}
	// */

	g.fillAll(Colours::white);
	SectionTypes::Section& rootSection = sectionManager.getRootSection();
	Rectangle<int> targetBounds = getLocalBounds();
	drawSection(g, rootSection, targetBounds);
}

void UniformsBarComponent::drawSection(Graphics& g, SectionTypes::Section& section, Rectangle<int>& targetBounds) const {
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;

	// draw section header
	const String sectionName = sectionManager.getSectionName(section);
	if (sectionName.isNotEmpty()) {
		// don't draw root section header
		const Rectangle<int> headerRect = targetBounds.removeFromTop(rowHeight);
		g.setColour(Colours::black);
		g.drawHorizontalLine(headerRect.getCentreY(), headerRect.getX() + 3, headerRect.getX() + 10);
		g.drawFittedText(sectionName, headerRect.withTrimmedLeft(13), Justification::centredLeft, 1);
		//g.drawHorizontalLine(headerRect.getCentreY(), 0, 0);
	}

	if (sectionManager.getSectionCollapsed(section)) {
		// the section is collapsed
		return;
	}

	// recursivly draw subsections
	const int numSections = sectionManager.getNumSections(section);
	for (int i = 0; i < numSections; i++) {
		SectionTypes::Section subsection = sectionManager.getSection(section, i);
		drawSection(g, subsection, targetBounds);
	}

	// draw uniforms
	const int numUniforms = sectionManager.getNumUniforms(section);
	for (int i = 0; i < numUniforms; i++) {
		const SectionTypes::Uniform uniform = sectionManager.getUniform(section, i);
		const String uniformName = sectionManager.getUniformName(uniform);
		const Rectangle<int> uniformRect = targetBounds.removeFromTop(rowHeight);
		g.drawFittedText(uniformName, uniformRect.withTrimmedLeft(3), Justification::centredLeft, 1);
	}
}

void UniformsBarComponent::mouseUp(const MouseEvent& event) {
	if (event.x < 0 || event.x >= getWidth()) {
		// click outside of uniformsBar
		return;
	}

	const int editorWidth = 300;
	const int rowHeight = 20;

	const int numUniform = int(event.position.y / float(rowHeight)); // floor division
	if (numUniform < 0 || numUniform >= data.getNumUniforms()) {
		// clicked on empty area at bottom of UniformsBar
		return;
	}

	const ModifierKeys& m = event.mods;
	if (event.mouseWasClicked() && m.isMiddleButtonDown() && m.isCommandDown()) {
		// bake uniform
		ValueTree uniformData = data.getUniform(numUniform);
		const int numSequences = data.getNumSequences(uniformData);

		if (numSequences != 0) {
			AlertWindow cantBakeWindow("Bake Uniform failed", "There are some Sequences left for this Uniform. Don't wanna burn those! Delete them before Baking!", AlertWindow::WarningIcon);
			cantBakeWindow.addButton("Cancel", 0, KeyPress('c'), KeyPress(KeyPress::escapeKey));
			cantBakeWindow.addButton(L"Fuck 'ëm", 1, KeyPress('f'), KeyPress(KeyPress::spaceKey));

			const int returnedChoice = cantBakeWindow.runModalLoop();
			if (returnedChoice != 1) {
				return;
			}
		}

		data.getUndoManager().beginNewTransaction("Bake Uniform");
		data.bakeUniform(uniformData);

	} else if (event.mouseWasClicked() && m.isLeftButtonDown()) {
		// show standard value editor
		ValueTree uniformData = data.getUniform(numUniform);
		const String uniformName = data.getUniformName(uniformData);
		ValueTree valueData = data.getUniformStandardValue(uniformData);
		jassert(valueData.isValid());

		PropertyComponent* valueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(uniformName, valueData);
		valueEditor->setSize(editorWidth, valueEditor->getPreferredHeight());

		// bounding rectangle of this uniform
		const Rectangle<int> rect(0, numUniform * rowHeight, getWidth(), rowHeight);
		CallOutBox& callOutBox = CallOutBox::launchAsynchronously(valueEditor, localAreaToGlobal(rect), nullptr);
		callOutBox.setDismissalMouseClicksAreAlwaysConsumed(true);
	}
}

void UniformsBarComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
}

void UniformsBarComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (childWhichHasBeenAdded.hasType(treeId::uniform)) {
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved, int /*indexFromWhichChildWasRemoved*/) {
	if (childWhichHasBeenRemoved.hasType(treeId::uniform)) {
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildOrderChanged(ValueTree& parentTree, int /*oldIndex*/, int /*newIndex*/) {
	if (parentTree == data.getUniformsArray()) {
		repaint();
	}
}

void UniformsBarComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void UniformsBarComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always the root tree
	updateSize();
	repaint();
}

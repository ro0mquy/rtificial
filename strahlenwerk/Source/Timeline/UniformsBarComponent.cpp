#include "UniformsBarComponent.h"

#include "TimelineData.h"
#include <Sidebar/ValueEditorPropertyComponent.h>
#include <RtificialLookAndFeel.h>
#include <Timeline/SectionManager.h>
#include <MainWindow.h>

UniformsBarComponent::UniformsBarComponent(SectionManager& sectionManager_) :
	data(TimelineData::getTimelineData()),
	sectionManager(sectionManager_)
{
	sectionManager.addListenerToTree(this);
}

UniformsBarComponent::~UniformsBarComponent() {
	sectionManager.removeListenerFromTree(this);
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

	// draw stripes
	const int myWidth = getWidth();
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	const int totalRows = sectionManager.getTotalHeightInRows();
	for (int i = 0; i < totalRows; i++) {
		g.setColour(findColour(i%2 == 0 ? UniformsBarComponent::evenRowColourId : UniformsBarComponent::oddRowColourId));
		g.fillRect(0, i*rowHeight, myWidth, rowHeight);

		g.setColour(findColour(UniformsBarComponent::uniformSeperatorColourId));
		g.drawHorizontalLine((i + 1) * rowHeight - 1, 0, myWidth);
	}

	SectionTypes::Section& rootSection = sectionManager.getRootSection();
	Rectangle<int> targetBounds = getLocalBounds();
	drawSection(g, rootSection, targetBounds);
}

void UniformsBarComponent::drawSection(Graphics& g, SectionTypes::Section& section, Rectangle<int>& targetBounds) const {
	// some constants
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;
	const int padding = 5;
	//const int firstLineWidth = 7;
	const int arrowEnd = 12;

	// draw section header
	const String sectionName = sectionManager.getSectionName(section);
	if (sectionName.isNotEmpty()) {
		// don't draw root section header

		const Colour sectionColor = RtificialLookAndFeel::getColorFromSectionName(sectionName);

		Rectangle<int> headerRect = targetBounds.removeFromTop(rowHeight);
		targetBounds.removeFromLeft(arrowEnd); // inset all subthingies

		// draw arrow
		const float arrowSize = (arrowEnd - padding) * 2.f / std::sqrt(3.f);
		Path arrowPath;
		arrowPath.startNewSubPath(0.0f, -0.5f * arrowSize);
		arrowPath.lineTo(0.0f, 0.5f * arrowSize);
		arrowPath.lineTo(std::sqrt(3.f)/2.f * arrowSize, 0.0f);
		arrowPath.closeSubPath();

		const bool isCollapsed = sectionManager.getSectionCollapsed(section);
		AffineTransform transformation;
		transformation = transformation.rotated(isCollapsed ? 0.f : .5f*float_Pi, .5f / std::sqrt(3.f) * arrowSize, 0.f);
		transformation = transformation.translated(headerRect.getX() + padding, headerRect.getCentreY());

		//g.setColour(findColour(UniformsBarComponent::sectionHeaderSeperatorColourId));
		g.setColour(sectionColor);
		g.fillPath(arrowPath, transformation);
		headerRect.removeFromLeft(arrowEnd + padding);

		/*
		// draw first line
		g.setColour(findColour(UniformsBarComponent::sectionHeaderSeperatorColourId));
		g.drawHorizontalLine(headerRect.getCentreY(), headerRect.getX(), headerRect.getX() + firstLineWidth);
		headerRect.removeFromLeft(firstLineWidth + padding);
		// */

		// fit and draw name
		GlyphArrangement sectionNameGlyphs;
		sectionNameGlyphs.addFittedText(g.getCurrentFont(), sectionName,
			   headerRect.getX(), headerRect.getY(), headerRect.getWidth(), headerRect.getHeight(),
			   Justification::centredLeft, 1);
		g.setColour(findColour(UniformsBarComponent::sectionHeaderTextColourId));
		sectionNameGlyphs.draw(g);

		// draw second line
		const Rectangle<float> sectionNameBounding = sectionNameGlyphs.getBoundingBox(0, -1, true);
		headerRect.setLeft(sectionNameBounding.getRight() + padding);
		//g.setColour(findColour(UniformsBarComponent::sectionHeaderSeperatorColourId));
		//g.drawLine(headerRect.getX(), headerRect.getCentreY(), headerRect.getRight(), headerRect.getCentreY(), 4.0f);
		g.setColour(sectionColor.withAlpha(0.35f));
		const int sectionLineHeight = 4;
		const Rectangle<int> sectionLine(
			headerRect.getX(),
			headerRect.getY() + ( (headerRect.getHeight() - sectionLineHeight) / 2 ),
			headerRect.getWidth(),
			sectionLineHeight
		);
		g.fillRect(sectionLine);
	}

	if (sectionManager.getSectionCollapsed(section)) {
		// the section is collapsed
		return;
	}

	// recursivly draw subsections
	const int numSections = sectionManager.getNumSections(section);
	for (int i = 0; i < numSections; i++) {
		SectionTypes::Section subsection = sectionManager.getSection(section, i);

		// draw subsections and restore left edge
		const int originalLeft = targetBounds.getX();
		drawSection(g, subsection, targetBounds);
		targetBounds.setLeft(originalLeft);
	}

	// draw uniforms
	g.setColour(findColour(UniformsBarComponent::uniformTextColourId));
	const int numUniforms = sectionManager.getNumUniforms(section);
	for (int i = 0; i < numUniforms; i++) {
		const SectionTypes::Uniform uniform = sectionManager.getUniform(section, i);
		const String uniformName = sectionManager.getUniformName(uniform);
		const Rectangle<int> uniformRect = targetBounds.removeFromTop(rowHeight);
		g.drawFittedText(uniformName, uniformRect.withTrimmedLeft(padding), Justification::centredLeft, 1);
	}
}

void UniformsBarComponent::mouseUp(const MouseEvent& event) {
	if (event.x < 0 || event.x >= getWidth()) {
		// click outside of uniformsBar
		return;
	}

	const int editorWidth = 300;
	const int rowHeight = RtificialLookAndFeel::uniformRowHeight;

	const int numRow = int(event.position.y / float(rowHeight)); // floor division
	if (numRow < 0) {
		return;
	}

	ValueTree uniformSectionTree = sectionManager.getUniformOrSectionForYPos(numRow);
	if (! uniformSectionTree.isValid()) {
		// click in empty area
		return;
	}

	if (sectionManager.isUniform(uniformSectionTree)) {
		// click on uniform
		const var uniformName = sectionManager.getUniformName(uniformSectionTree);
		ValueTree uniformData = data.getUniform(uniformName);
		jassert(uniformData.isValid());

		const ModifierKeys& m = event.mods;
		if (event.mouseWasClicked() && m.isMiddleButtonDown() && m.isCommandDown()) {
			// bake uniform
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
			ValueTree valueData = data.getUniformStandardValue(uniformData);
			jassert(valueData.isValid());

			PropertyComponent* valueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(uniformName, valueData);
			valueEditor->setSize(editorWidth, valueEditor->getPreferredHeight());

			// bounding rectangle of this uniform
			const Rectangle<int> rect(0, numRow * rowHeight, getWidth(), rowHeight);
			CallOutBox& callOutBox = CallOutBox::launchAsynchronously(valueEditor, localAreaToGlobal(rect), nullptr);
			callOutBox.setDismissalMouseClicksAreAlwaysConsumed(true);
			callOutBox.addKeyListener(MainWindow::getApplicationCommandManager().getKeyMappings());
		}
	} else if (sectionManager.isSection(uniformSectionTree)) {
		// click on section

		const ModifierKeys& m = event.mods;
		if (event.mouseWasClicked() && m.isLeftButtonDown()) {
			// toggle section collapsed status
			sectionManager.toggleSectionCollapsed(uniformSectionTree);
		}
	}
}

void UniformsBarComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& property) {
	if (property == sectionTreeId::sectionCollapsed) {
		// section collapsed state changed
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (sectionManager.isSection(childWhichHasBeenAdded) || sectionManager.isUniform(childWhichHasBeenAdded)) {
		// section or uniform added
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenRemoved, int /*indexFromWhichChildWasRemoved*/) {
	if (sectionManager.isSection(childWhichHasBeenRemoved) || sectionManager.isUniform(childWhichHasBeenRemoved)) {
		// section or uniform removed
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildOrderChanged(ValueTree& parentTree, int /*oldIndex*/, int /*newIndex*/) {
	if (parentTree.hasType(sectionTreeId::sectionsArray) || parentTree.hasType(sectionTreeId::uniformsArray)) {
		// the ordering of some sections or uniforms has changed
		repaint();
	}
}

void UniformsBarComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

void UniformsBarComponent::valueTreeRedirected(ValueTree& /*treeWhoWasRedirected*/) {
	// always the root section
	updateSize();
	repaint();
}

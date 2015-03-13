#include "UniformsBarComponent.h"

#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include <Sidebar/ValueEditorPropertyComponent.h>

UniformsBarComponent::UniformsBarComponent() :
	data(TimelineData::getTimelineData())
{
	data.addListenerToTree(this);
}

UniformsBarComponent::~UniformsBarComponent() {
	data.removeListenerFromTree(this);
}

void UniformsBarComponent::updateSize() {
	const int rowHeight = 20;
	const int numUniforms = data.getNumUniforms();

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int height = jmax(numUniforms * rowHeight + 18, viewportHeight); // TODO: fix 18
	setSize(viewportWidth, height);
}

void UniformsBarComponent::paint(Graphics& g) {
	const int rowHeight = 20;
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
		g.drawText(name, rect.withLeft(3), Justification(Justification::centredLeft), true);
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

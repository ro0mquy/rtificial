#include "UniformsBarComponent.h"

#include "TreeIdentifiers.h"
#include "ValueEditorPropertyComponent.h"
#include "../RtColourIds.h"
#include "TimelineData.h"

UniformsBarComponent::UniformsBarComponent() :
	data(TimelineData::getTimelineData())
{
	data.addListenerToTree(this);
}

void UniformsBarComponent::updateSize() {
	const int rowHeight = 20;
	const int height = jmax(data.getUniformsArray().getNumChildren() * rowHeight, getParentHeight());
	setSize(getWidth(), height);
}

void UniformsBarComponent::paint(Graphics& g) {
	const int rowHeight = 20;
	ValueTree uniformsArray = data.getUniformsArray();
	const int numChildren = uniformsArray.getNumChildren();

	for(int i = 0; i < numChildren; i++) {
		ValueTree uniform = uniformsArray.getChild(i);
		const String name = uniform.getProperty(treeId::uniformName);

		const Rectangle<float> rect(0, i*rowHeight, getWidth(), rowHeight);

		g.setColour(findColour(i%2 == 0 ? UniformsBarComponent::evenRowColourId : UniformsBarComponent::oddRowColourId));
		g.fillRect(rect);
		g.setColour(findColour(UniformsBarComponent::seperatorColourId));
		g.drawHorizontalLine(i*rowHeight+rowHeight-1, 0, getWidth());
		g.setColour(findColour(UniformsBarComponent::uniformTextColourId));
		g.drawText(name, rect.withLeft(3), Justification(Justification::centredLeft), true);
	}

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawVerticalLine(getWidth()-1, 0, getHeight());
}

void UniformsBarComponent::mouseUp(const MouseEvent& event) {
	const int editorWidth = 300;
	const int rowHeight = 20;

	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
	ValueTree uniformData = data.getUniformsArray().getChild(numUniform);
	const String uniformName = uniformData.getProperty(treeId::uniformName);
	ValueTree valueData = uniformData.getChildWithName(treeId::uniformStandardValue);
	if (!valueData.isValid()) return;

	PropertyComponent* valueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(uniformName, valueData);
	valueEditor->setSize(editorWidth, valueEditor->getPreferredHeight());

	const Rectangle<int> rect(0, numUniform * rowHeight, getWidth(), rowHeight);
	CallOutBox::launchAsynchronously(valueEditor, localAreaToGlobal(rect), nullptr);
}

void UniformsBarComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
}

void UniformsBarComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {
	if (childWhichHasBeenAdded.hasType(treeId::uniform)) {
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {
}

void UniformsBarComponent::valueTreeChildOrderChanged(ValueTree& parentTree) {
}

void UniformsBarComponent::valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) {
}

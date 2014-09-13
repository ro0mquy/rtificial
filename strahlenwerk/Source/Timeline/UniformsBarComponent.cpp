#include "UniformsBarComponent.h"

#include <RtColourIds.h>
#include "TimelineData.h"
#include "ValueEditorPropertyComponent.h"

UniformsBarComponent::UniformsBarComponent() :
	data(TimelineData::getTimelineData())
{
	data.addListenerToTree(this);
}

void UniformsBarComponent::updateSize() {
	const int rowHeight = 20;
	const int numUniforms = data.getNumUniforms();

	const Viewport* parentViewport = findParentComponentOfClass<Viewport>();
	const int viewportWidth = parentViewport->getMaximumVisibleWidth();
	const int viewportHeight = parentViewport->getMaximumVisibleHeight();

	const int height = jmax(numUniforms * rowHeight, viewportHeight);
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

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawVerticalLine(getWidth()-1, 0, getHeight());
}

void UniformsBarComponent::mouseUp(const MouseEvent& event) {
	const int editorWidth = 300;
	const int rowHeight = 20;

	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight)); // floor division
	if (numUniform >= data.getNumUniforms()) {
		// clicked on empty area at bottom of UniformsBar
		return;
	}

	ValueTree uniformData = data.getUniform(numUniform);
	const String uniformName = data.getUniformName(uniformData);
	ValueTree valueData = data.getUniformStandardValue(uniformData);
	jassert(valueData.isValid());

	PropertyComponent* valueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(uniformName, valueData);
	valueEditor->setSize(editorWidth, valueEditor->getPreferredHeight());

	// bounding rectangle of this uniform
	const Rectangle<int> rect(0, numUniform * rowHeight, getWidth(), rowHeight);
	CallOutBox::launchAsynchronously(valueEditor, localAreaToGlobal(rect), nullptr);
}

void UniformsBarComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) {
}

void UniformsBarComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& childWhichHasBeenAdded) {
	if (data.isUniform(childWhichHasBeenAdded)) {
		updateSize();
		repaint();
	}
}

void UniformsBarComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/) {
}

void UniformsBarComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/) {
}

void UniformsBarComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

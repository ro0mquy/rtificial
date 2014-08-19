#include "UniformsBarComponent.h"
#include "TreeIdentifiers.h"
#include "ValueEditorComponent.h"
#include "../RtColourIds.h"

UniformsBarComponent::UniformsBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
}

void UniformsBarComponent::updateSize() {
	const int height = jmax(data.getUniformsArray().getNumChildren() * 20, getParentHeight());
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
	const int rowHeight = 20;
	const int numUniform = int(float(event.getMouseDownY()) / float(rowHeight));
	ValueTree valueData = data.getUniformsArray().getChild(numUniform).getChildWithName(treeId::uniformStandardValue);
	if (!valueData.isValid()) return;

	ValueEditorComponent* valueEditor = new ValueEditorComponent(valueData);
	valueEditor->setSize(200, 200);

	const Rectangle<int> rect(0, numUniform * rowHeight, getWidth(), rowHeight);
	CallOutBox::launchAsynchronously(valueEditor, localAreaToGlobal(rect), nullptr);
}

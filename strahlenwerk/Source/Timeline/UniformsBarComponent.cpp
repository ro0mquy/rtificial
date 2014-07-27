#include "UniformsBarComponent.h"
#include "TreeIdentifiers.h"
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
	ValueTree uniformsArray = data.getUniformsArray();
	const int numChildren = uniformsArray.getNumChildren();

	for(int i = 0; i < numChildren; i++){
		ValueTree uniform = uniformsArray.getChild(i);
		const String name = uniform.getProperty(treeId::uniformName);

		const Rectangle<float> rect(0, i*20, getWidth(), 20);

		g.setColour(findColour(i%2 == 0 ? UniformsBarComponent::evenRowColourId : UniformsBarComponent::oddRowColourId));
		g.fillRect(rect);
		g.setColour(findColour(UniformsBarComponent::seperatorColourId));
		g.drawHorizontalLine(i*20+20-1, 0, getWidth());
		g.setColour(findColour(UniformsBarComponent::uniformTextColourId));
		g.drawText(name, rect.withLeft(3), Justification(Justification::centredLeft), true);
	}

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawVerticalLine(getWidth()-1, 0, getHeight());
}

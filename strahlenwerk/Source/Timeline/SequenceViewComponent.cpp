#include "SequenceViewComponent.h"

SequenceViewComponent::SequenceViewComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
}

void SequenceViewComponent::resized() {
	int width = jmax(data.getLastSceneEndTime() + 20, getParentWidth());
	int height = jmax(data.getUniformsArray().getNumChildren() * 20, getParentHeight());
	setSize(width, height);
}

void SequenceViewComponent::paint(Graphics& g){
	ValueTree uniformsArray = data.getUniformsArray();
	int numChildren = uniformsArray.getNumChildren();

	for(int i = 0; i < numChildren; i++){
		ValueTree uniform = uniformsArray.getChild(i);
		Rectangle<float> rect(0, i*20, getWidth(), 20);
		g.setColour(findColour(i%2 == 0 ? SequenceViewComponent::evenRowColourId : SequenceViewComponent::oddRowColourId));
		g.fillRect(rect);
		g.setColour(findColour(SequenceViewComponent::seperatorColourId));
		g.drawHorizontalLine(i*20+20-1, 0, getWidth());
	}

	// draw time marker
	g.setColour(findColour(SequenceViewComponent::timeMarkerColourId));
	float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);
}

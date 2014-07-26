#include "ScenesBar.h"
#include "../RtificialLookAndFeel.h"

ScenesBarComponent::ScenesBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
	updateSceneComponents();
}

void ScenesBarComponent::resized() {
	const int width = jmax(data.getLastSceneEndTime() + 20, getParentWidth());
	setSize(width, getHeight());
}

void ScenesBarComponent::paint(Graphics& g) {
	// höhö G-Punkt

	// draw ticks
	g.setColour(findColour(ScenesBarComponent::tickColourId));

	const int lineDistance             = 20;
	const int longLineDistance         = 5;
	const float lineHeightFraction     = 0.25;
	const float longLineHeightFraction = 0.5;

	for(int i = 0; i*lineDistance < getWidth(); i++){
		const bool longLine = (i%longLineDistance == 0);
		g.drawLine(
				i*lineDistance + 0.5,
				0,
				i*lineDistance + 0.5,
				getHeight()*(longLine ? longLineHeightFraction : lineHeightFraction),
				1
			);
	}

	// draw time marker
	g.setColour(findColour(ScenesBarComponent::timeMarkerColourId));
	const float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);

	// draw outline
	g.setColour(findColour(RtColourIds::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
}

void ScenesBarComponent::updateSceneComponents() {
	sceneComponentsArray.clearQuick(true);
	ValueTree sceneDatasArray = data.getScenesArray();
	const int numScenes = sceneDatasArray.getNumChildren();

	for (int i = 0; i < numScenes; i++) {
		ValueTree sceneData = sceneDatasArray.getChild(i);
		SceneComponent* sceneComponent = new SceneComponent(sceneData);
		addAndMakeVisible(sceneComponent);
		sceneComponentsArray.add(sceneComponent);
	}
}
#include "TreeIdentifiers.h"
#include "SceneComponent.h"

SceneComponent::SceneComponent(ValueTree _sceneData) :
	sceneData(_sceneData)
{
	// start timer, so the bounds get set,
	// when this component was added to the ScenesBarComponent
	startTimer(0);
}

void SceneComponent::timerCallback() {
	stopTimer();

	// set bounds
	int start = sceneData.getProperty(treeId::sceneStart);
	int duration = sceneData.getProperty(treeId::sceneDuration);

	float padding = 0;
	setBounds(start, padding, duration, getParentHeight() - 2*padding);
}

void SceneComponent::paint(Graphics& g) {
	float cornerSize = 5.0;
	Rectangle<float> rect = getLocalBounds().toFloat();
	rect.removeFromTop(.5);
	rect.removeFromBottom(1.5);

	String shaderSourceName = sceneData.getProperty(treeId::sceneShaderSource);
	g.setColour(findColour(SceneComponent::fillColourId));
	g.fillRoundedRectangle(rect, cornerSize);

	g.setColour(findColour(SceneComponent::outlineColourId));
	g.drawRoundedRectangle(rect, cornerSize, 1);

	g.setColour(findColour(SceneComponent::textColourId));
	g.drawText(shaderSourceName, rect, Justification(Justification::centred), true);
}

#include "ScenesBar.h"
#include "TreeIdentifiers.h"
#include "../RtificialLookAndFeel.h"

ScenesBarComponent::ScenesBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
	updateSceneComponents();
}

void ScenesBarComponent::resized() {
	int width = jmax(data.getLastSceneEndTime() + 20, getParentWidth());
	setSize(width, getHeight());
}

void ScenesBarComponent::paint(Graphics& g) {
	// höhö G-Punkt

	// draw ticks
	g.setColour(findColour(ScenesBarComponent::tickColourId));

	int lineDistance             = 20;
	int longLineDistance         = 5;
	float lineHeightFraction     = 0.25;
	float longLineHeightFraction = 0.5;

	for(int i = 0; i*lineDistance < getWidth(); i++){
		bool longLine = (i%longLineDistance == 0);
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
	float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);

	// draw outline
	g.setColour(findColour(ExtRtLAF::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
}

void ScenesBarComponent::updateSceneComponents() {
	sceneComponentsArray.clearQuick(true);
	ValueTree sceneDatasArray = data.getScenesArray();
	int numScenes = sceneDatasArray.getNumChildren();

	for (int i = 0; i < numScenes; i++) {
		ValueTree sceneData = sceneDatasArray.getChild(i);
		SceneComponent* sceneComponent = new SceneComponent(sceneData);
		addAndMakeVisible(sceneComponent);
		sceneComponentsArray.add(sceneComponent);
	}
}

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

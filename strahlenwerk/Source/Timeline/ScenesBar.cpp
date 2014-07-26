#include "ScenesBar.h"
#include "TreeIdentifiers.h"
#include "../RtificialLookAndFeel.h"

ScenesBarComponent::ScenesBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
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

	//draw scenes
	ValueTree scenesArray = data.getScenesArray();
	int numChildren = scenesArray.getNumChildren();

	for (int i = 0; i < numChildren; i++) {
		ValueTree scene = scenesArray.getChild(i);
		int start = scene.getProperty(treeId::sceneStart);
		int duration = scene.getProperty(treeId::sceneDuration);
		String shaderSourceName = scene.getProperty(treeId::sceneShaderSource);

		float cornerSize = 5.0f;
		float padding = 0;
		Rectangle<float> sceneRect(start+0.5, 0.5+padding, duration, getHeight()-1-1-2*padding);

		g.setColour(findColour(ScenesBarComponent::sceneColourId));
		g.fillRoundedRectangle(sceneRect, cornerSize);

		g.setColour(findColour(ScenesBarComponent::sceneOutlineColourId));
		g.drawRoundedRectangle(sceneRect, cornerSize, 1);

		g.setColour(findColour(ScenesBarComponent::sceneTextColourId));
		g.drawText(shaderSourceName, sceneRect, Justification(Justification::centred), true);
	}

	// draw time marker
	g.setColour(findColour(ScenesBarComponent::timeMarkerColourId));
	float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);

	// draw outline
	g.setColour(findColour(ExtRtLAF::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
}


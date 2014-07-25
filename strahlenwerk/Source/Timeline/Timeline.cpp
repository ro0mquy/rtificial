#include "Timeline.h"
#include "../RtificialLookAndFeel.h"

// functions of the allmighty Timeline class
Timeline::Timeline() :
	currentTime(50),
	data(),
	viewportCanvas(*this),
	viewportScenes(*this),
	viewportUniforms(*this),
	sequenceView(currentTime, data),
	scenesBar(currentTime, data),
	uniformsBar(currentTime, data)
{
	viewportCanvas.setViewedComponent(&sequenceView, false);
	viewportScenes.setViewedComponent(&scenesBar, false);
	viewportScenes.setScrollBarsShown(false, false, false, true);
	viewportUniforms.setViewedComponent(&uniformsBar, false);
	viewportUniforms.setScrollBarsShown(false, false, true, false);
	addAndMakeVisible(viewportCanvas);
	addAndMakeVisible(viewportScenes);
	addAndMakeVisible(viewportUniforms);
}

void Timeline::resized() {
	int scenesComponentHeight = 30;
	int uniformComponenentWidth = 30;
	Rectangle<int> r(getLocalBounds());

	viewportScenes.setBounds(
			r.removeFromTop(scenesComponentHeight)
			.withTrimmedLeft(uniformComponenentWidth)
		);
	scenesBar.setSize(
			scenesBar.getWidth(),
			scenesComponentHeight
		);
	scenesBar.resized();

	viewportUniforms.setBounds(r.removeFromLeft(uniformComponenentWidth));
	uniformsBar.setSize(
			uniformComponenentWidth,
			uniformsBar.getHeight()
		);
	uniformsBar.resized();

	viewportCanvas.setBounds(r);
	sequenceView.resized();
}

// gets called when one of the viewports changed
// syncs the current position between all three viewports
void Timeline::callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position) {
	if (vp == &viewportCanvas) {
		// X and Y have changed
		viewportScenes.setViewPosition(
				position.getX(),
				viewportScenes.getViewPositionY()
				);
		viewportUniforms.setViewPosition(
				viewportUniforms.getViewPositionX(),
				position.getY()
				);
	} else if (vp == &viewportScenes) {
		// only X has changed
		viewportCanvas.setViewPosition(
				position.getX(),
				viewportCanvas.getViewPositionY()
				);
	} else if (vp == &viewportUniforms) {
		// only Y has changed
		viewportCanvas.setViewPosition(
				viewportCanvas.getViewPositionX(),
				position.getY()
				);
	}
}

ScenesBarComponent::ScenesBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
}

void ScenesBarComponent::resized() {
	Timeline::ViewportCallback* parentViewport = findParentComponentOfClass<Timeline::ViewportCallback>();
	int viewportWidth = parentViewport->getMaximumVisibleWidth();
	int componentWidth = jmax(data.getLastSceneEndTime() + 20, viewportWidth);
	setSize(componentWidth, getHeight());
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

		// hackish, pleace fix using treeId::
		Identifier sceneStart("sceneStart");
		Identifier sceneDuration("sceneDuration");
		Identifier sceneShaderSource("sceneShaderSource");

		int start = scene.getProperty(sceneStart);
		int duration = scene.getProperty(sceneDuration);
		String shaderSourceName = scene.getProperty(sceneShaderSource);
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
	g.setColour(findColour(RtificialLookAndFeel::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
}

UniformsBarComponent::UniformsBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
}

void UniformsBarComponent::paint(Graphics& g) {
}

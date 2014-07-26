#include "Timeline.h"
#include "TreeIdentifiers.h"
#include "../RtificialLookAndFeel.h"

// functions of the allmighty Timeline class
Timeline::Timeline() :
	currentTime(50),
	data(),
	viewportSequenceView(*this),
	viewportScenesBar(*this),
	viewportUniformsBar(*this),
	sequenceView(currentTime, data),
	scenesBar(currentTime, data),
	uniformsBar(currentTime, data)
{
	viewportSequenceView.setViewedComponent(&sequenceView, false);
	viewportScenesBar.setViewedComponent(&scenesBar, false);
	viewportScenesBar.setScrollBarsShown(false, false, false, true);
	viewportUniformsBar.setViewedComponent(&uniformsBar, false);
	viewportUniformsBar.setScrollBarsShown(false, false, true, false);
	addAndMakeVisible(viewportSequenceView);
	addAndMakeVisible(viewportScenesBar);
	addAndMakeVisible(viewportUniformsBar);
	resized();
}

void Timeline::resized() {
	int scenesBarHeight = 30;
	int uniformsBarWidth = 150;
	Rectangle<int> r(getLocalBounds());

	viewportScenesBar.setBounds(
			r.removeFromTop(scenesBarHeight)
			.withTrimmedLeft(uniformsBarWidth)
		);
	scenesBar.setSize(
			scenesBar.getWidth(),
			scenesBarHeight
		);
	scenesBar.resized();

	viewportUniformsBar.setBounds(r.removeFromLeft(uniformsBarWidth));
	uniformsBar.setSize(
			uniformsBarWidth,
			uniformsBar.getHeight()
		);
	uniformsBar.resized();

	viewportSequenceView.setBounds(r);
	sequenceView.resized();
}

// gets called when one of the viewports changed
// syncs the current position between all three viewports
void Timeline::callbackViewportChanged(Timeline::ViewportCallback* vp, Point<int> position) {
	if (vp == &viewportSequenceView) {
		// X and Y have changed
		viewportScenesBar.setViewPosition(
				position.getX(),
				viewportScenesBar.getViewPositionY()
				);
		viewportUniformsBar.setViewPosition(
				viewportUniformsBar.getViewPositionX(),
				position.getY()
				);
	} else if (vp == &viewportScenesBar) {
		// only X has changed
		viewportSequenceView.setViewPosition(
				position.getX(),
				viewportSequenceView.getViewPositionY()
				);
	} else if (vp == &viewportUniformsBar) {
		// only Y has changed
		viewportSequenceView.setViewPosition(
				viewportSequenceView.getViewPositionX(),
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
	g.setColour(findColour(RtificialLookAndFeel::outlineColourId));
	g.drawHorizontalLine(getHeight()-1, 0, getWidth());
}

UniformsBarComponent::UniformsBarComponent(Value& timeValue, Data& _data) :
	currentTime(timeValue),
	data(_data)
{
}

void UniformsBarComponent::resized() {
	int height = jmax(data.getUniformsArray().getNumChildren() * 20, getParentHeight());
	setSize(getWidth(), height);
}

void UniformsBarComponent::paint(Graphics& g) {
	ValueTree uniformsArray = data.getUniformsArray();
	int numChildren = uniformsArray.getNumChildren();

	for(int i = 0; i < numChildren; i++){
		ValueTree uniform = uniformsArray.getChild(i);
		String name = uniform.getProperty(treeId::uniformName);

		Rectangle<float> rect(0, i*20, getWidth(), 20);

		g.setColour(findColour(i%2 == 0 ? UniformsBarComponent::evenRowColourId : UniformsBarComponent::oddRowColourId));
		g.fillRect(rect);
		g.setColour(findColour(UniformsBarComponent::seperatorColourId));
		g.drawHorizontalLine(i*20+20-1, 0, getWidth());
		g.setColour(findColour(UniformsBarComponent::uniformTextColourId));
		g.drawText(name, rect.withLeft(3), Justification(Justification::centredLeft), true);
	}

	// draw outline
	g.setColour(findColour(RtificialLookAndFeel::outlineColourId));
	g.drawVerticalLine(getWidth()-1, 0, getHeight());
}

#include "Timeline.h"
#include "TimelineTree.h"

// functions of the allmighty Timeline class
Timeline::Timeline() :
	currentTime(50),
	valueTree(ttId::timelineTree),
	viewportCanvas(*this),
	viewportScenes(*this),
	viewportUniforms(*this),
	componentCanvas(currentTime),
	componentScenes(currentTime),
	componentUniforms(currentTime)
{
	viewportCanvas.setViewedComponent(&componentCanvas, false);
	viewportScenes.setViewedComponent(&componentScenes, false);
	viewportScenes.setScrollBarsShown(false, false, false, true);
	viewportUniforms.setViewedComponent(&componentUniforms, false);
	viewportUniforms.setScrollBarsShown(false, false, true, false);
	addAndMakeVisible(viewportCanvas);
	addAndMakeVisible(viewportScenes);
	addAndMakeVisible(viewportUniforms);

	setupValueTree();
}

void Timeline::resized() {
	int scenesComponentHeight = 30;
	int uniformComponenentWidth = 30;
	Rectangle<int> r(getLocalBounds());

	viewportScenes.setBounds(
			r.removeFromTop(scenesComponentHeight)
			.withTrimmedLeft(uniformComponenentWidth)
		);
	componentScenes.setSize(
			componentScenes.getWidth(),
			scenesComponentHeight
		);

	viewportUniforms.setBounds(r.removeFromLeft(uniformComponenentWidth));
	componentUniforms.setSize(
			uniformComponenentWidth,
			componentUniforms.getHeight()
		);

	viewportCanvas.setBounds(r);
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

void Timeline::setupValueTree() {
	ValueTree scenesArray(ttId::scenesArray);
	for (int i = 0; i < 4; i++) {
		ValueTree scene(ttId::scene);
		scene.setProperty(ttId::sceneId, var(i), nullptr);
		scene.setProperty(ttId::sceneStart, var(60 * i), nullptr);
		scene.setProperty(ttId::sceneDuration, var(60 * i + 40), nullptr);
		scene.setProperty(ttId::sceneShaderSource, var(String("glsl") + String(i)), nullptr);
		scenesArray.addChild(scene, -1, nullptr);
	}
	valueTree.addChild(scenesArray, -1, nullptr);
}

Timeline::ViewportCallback::ViewportCallback(Timeline& timelineParent) :
	timelineParent(timelineParent)
{
}

// make a callback to Timeline if one of the viewports changed
void Timeline::ViewportCallback::visibleAreaChanged(const Rectangle<int>& newVisibleArea) {
	timelineParent.callbackViewportChanged(this, newVisibleArea.getPosition());
};

TimelineScenes::TimelineScenes(Value& timeValue) : currentTime(timeValue)
{
}

void TimelineScenes::resized() {
	setSize(1000, getHeight());
}
void TimelineScenes::paint(Graphics& g) {
	// höhö G-Punkt
	g.setColour(Colours::white);

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

	g.setColour(Colours::red);
	float x = currentTime.getValue();
	g.drawLine(x, 0, x, getHeight(), 2);
}

TimelineUniforms::TimelineUniforms(Value& timeValue) : currentTime(timeValue)
{
}

void TimelineUniforms::paint(Graphics& g) {
}

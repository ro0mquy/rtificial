#include "OpenGLTargetComponent.h"

#include "StrahlenwerkApplication.h"
#include "PropertyNames.h"
#include "Renderer.h"
#include "Timeline/Interpolator.h"
#include "Timeline/TreeIdentifiers.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

OpenGLTargetComponent::OpenGLTargetComponent(Renderer& _renderer) : renderer(_renderer) {
	setOpaque(true);
}

void OpenGLTargetComponent::paint(Graphics& g) {
	const Rectangle<int> bounds = getLocalBounds();
	if(StrahlenwerkApplication::getInstance()->getProperties().getBoolValue(PropertyNames::GridEnabled)) {
		const float thirdHoriz = bounds.getWidth() / 3.;
		const float thirdVert = bounds.getHeight() / 3.;
		const float thickness = bounds.getWidth() / 200.;
		const float border = .05;
		const Rectangle<int> reducedBounds = bounds.reduced(bounds.getWidth() * border, bounds.getHeight() * border);

		Path path;
		path.startNewSubPath(0, thirdVert);
		path.lineTo(bounds.getRight(), thirdVert);
		path.startNewSubPath(0, thirdVert * 2);
		path.lineTo(bounds.getRight(), thirdVert * 2);
		path.startNewSubPath(thirdHoriz, 0);
		path.lineTo(thirdHoriz, bounds.getBottom());
		path.startNewSubPath(thirdHoriz * 2, 0);
		path.lineTo(thirdHoriz * 2, bounds.getBottom());
		path.addRectangle(reducedBounds);

		g.setColour(Colours::white);
		g.setOpacity(.3);
		g.strokePath(path, PathStrokeType(thickness));
	}

	const float fontHeight = 12.;
	g.setColour(Colours::white);
	g.setFont(Font(Font::getDefaultMonospacedFontName(), fontHeight, 0));
	const std::uint64_t frameTime = renderer.getLastFrameDuration();
	const double frameTimeInMillis = frameTime / 1000000.;
	const String frameTimeString = String(frameTimeInMillis) + "ms\n" + String(1000. / frameTimeInMillis) + "fps";
	g.drawMultiLineText(frameTimeString, 10, 12 + fontHeight / 2, getWidth());

	// draw OriantedAxes (sic!)
	TimelineData& data = TimelineData::getTimelineData();
	Interpolator& interpolator = data.getInterpolator();

	ValueTree defaultRotation(treeId::interpolatedValue);
	data.initializeValue(defaultRotation, "quat");

	quat rotation = data.getQuatFromValue(interpolator.getCurrentUniformState("camera_rotation", defaultRotation).first);
	// display world rotation, not camera rotation
	rotation = conjugate(rotation);

	vec3 xAxis(1.f, 0.f, 0.f);
	vec3 yAxis(0.f, 1.f, 0.f);
	vec3 zAxis(0.f, 0.f, 1.f);
	xAxis = rotation * xAxis;
	yAxis = rotation * yAxis;
	zAxis = rotation * zAxis;

	const float axesWidth = bounds.getHeight() * .1f;
	const Point<float> centerPoint = bounds.getBottomLeft().translated(axesWidth * 1.5f, -axesWidth * 1.5f).toFloat();

	const Point<float> xTransformed(xAxis.x * axesWidth, -xAxis.y * axesWidth);
	const Point<float> yTransformed(yAxis.x * axesWidth, -yAxis.y * axesWidth);
	const Point<float> zTransformed(zAxis.x * axesWidth, -zAxis.y * axesWidth);

	const Point<float> xTip = centerPoint + xTransformed;
	const Point<float> yTip = centerPoint + yTransformed;
	const Point<float> zTip = centerPoint + zTransformed;

	g.setOpacity(1.f);
	// "Hinten dunkel vorne hell" "Ja, das ginge doch!"
	// Das ist zwar jetzt nicht schön hier und eigentlich immernoch total unmöglich zu erkennen, aber es ging schnell und tut!

	//g.setColour(Colour(0xffff0000));
	g.setGradientFill(ColourGradient(Colour(0xffff0000), centerPoint.x, centerPoint.y,
				xAxis.z < 0.f ? Colour(0xffff0000).darker(-xAxis.z) : Colour(0xffff0000).brighter(xAxis.z), xTip.x, xTip.y, false));
	g.drawLine(Line<float>(centerPoint, xTip), 4.f);
	const Point<float> xText = xTip + xTransformed * .1f;
	g.drawSingleLineText("x", xText.getX(), xText.getY());

	//g.setColour(Colour(0xff00ff00));
	g.setGradientFill(ColourGradient(Colour(0xff00ff00), centerPoint.x, centerPoint.y,
				yAxis.z < 0.f ? Colour(0xff00ff00).darker(-yAxis.z) : Colour(0xff00ff00).brighter(yAxis.z), yTip.x, yTip.y, false));
	const Point<float> yText = yTip + yTransformed * .1f;
	g.drawLine(Line<float>(centerPoint, yTip), 4.f);
	g.drawSingleLineText("y", yText.getX(), yText.getY());

	//g.setColour(Colour(0xff0000ff));
	g.setGradientFill(ColourGradient(Colour(0xff0000ff), centerPoint.x, centerPoint.y,
				zAxis.z < 0.f ? Colour(0xff0000ff).darker(-zAxis.z) : Colour(0xff0000ff).brighter(zAxis.z), zTip.x, zTip.y, false));
	const Point<float> zText = zTip + zTransformed * .1f;
	g.drawLine(Line<float>(centerPoint, zTip), 4.f);
	g.drawSingleLineText("z", zText.getX(), zText.getY());
}

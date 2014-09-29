#include "OpenGLTargetComponent.h"

#include "StrahlenwerkApplication.h"
#include "PropertyNames.h"
#include "Renderer.h"

OpenGLTargetComponent::OpenGLTargetComponent(Renderer& _renderer) : renderer(_renderer) {
	setOpaque(true);
}

void OpenGLTargetComponent::paint(Graphics& g) {
	const auto bounds = getLocalBounds();
	if(StrahlenwerkApplication::getInstance()->getProperties().getBoolValue(PropertyNames::GRID_ENABLED)) {
		const float thirdHoriz = bounds.getWidth() / 3.;
		const float thirdVert = bounds.getHeight() / 3.;
		const float thickness = bounds.getWidth() / 200.;
		const float border = .05;
		const auto reducedBounds = bounds.reduced(bounds.getWidth() * border, bounds.getHeight() * border);

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
	const uint64_t frameTime = renderer.getLastFrameDuration();
	const double frameTimeInMillis = frameTime / 1000000.;
	const String frameTimeString = String(frameTimeInMillis) + "ms\n" + String(1000. / frameTimeInMillis) + "fps";
	g.drawMultiLineText(frameTimeString, 10, 12 + fontHeight / 2, getWidth());
}

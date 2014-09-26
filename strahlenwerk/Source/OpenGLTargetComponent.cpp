#include "OpenGLTargetComponent.h"

#include "StrahlenwerkApplication.h"
#include "PropertyNames.h"

void OpenGLTargetComponent::paint(Graphics& g) {
	if(StrahlenwerkApplication::getInstance()->getProperties().getBoolValue(PropertyNames::GRID_ENABLED)) {
		const auto bounds = getLocalBounds();
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
}

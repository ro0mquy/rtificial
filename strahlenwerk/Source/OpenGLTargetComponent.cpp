#include "OpenGLTargetComponent.h"

#include "StrahlenwerkApplication.h"
#include "PropertyNames.h"

void OpenGLTargetComponent::paint(Graphics& g) {
	if(StrahlenwerkApplication::getInstance()->getProperties().getBoolValue(PropertyNames::GRID_ENABLED)) {
		// TODO remove from border
		const auto bounds = getLocalBounds();
		const float thirdHoriz = bounds.getWidth() / 3.;
		const float thirdVert = bounds.getHeight() / 3.;
		const float thickness = bounds.getWidth() / 200.;
		g.setColour(Colours::white);
		g.setOpacity(.3);
		g.drawLine(0., thirdVert, bounds.getWidth(), thirdVert, thickness);
		g.drawLine(0., thirdVert * 2, bounds.getWidth(), thirdVert * 2, thickness);
		g.drawLine(thirdHoriz, bounds.getHeight(), thirdHoriz, 0., thickness);
		g.drawLine(thirdHoriz * 2, bounds.getHeight(), thirdHoriz * 2, 0., thickness);
	}
}

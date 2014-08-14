#include "ColorSpaceMarker.h"

ColorSpaceMarker::ColorSpaceMarker() {
	setInterceptsMouseClicks (false, false);
}

void ColorSpaceMarker::paint(Graphics& g) {
	g.setColour(Colour::greyLevel(0.1f));
	g.drawEllipse(1., 1., getWidth() - 2., getHeight() - 2., 1.);
	g.setColour(Colour::greyLevel (0.9f));
	g.drawEllipse(2., 2., getWidth() - 4., getHeight() - 4., 1.);
}

#include "LabColorSpaceView.h"

const float LabColorSpaceView::abRange = 110.;

LabColorSpaceView::LabColorSpaceView(ColorChangedListener& listener, const LabColor& color) :
	edge(4),
	color(color),
	listener(listener)
{
	addAndMakeVisible(marker);
	setMouseCursor(MouseCursor::CrosshairCursor);
}

void LabColorSpaceView::paint(Graphics& g) {
	if(colors.isNull()) {
		const int width = getWidth();
		const int height = getHeight();
		colors = Image(Image::RGB, width, height, false);

		Image::BitmapData pixels(colors, Image::BitmapData::writeOnly);
		for(int y = 0; y < height; y++) {
			const float b = (1. - y / ( float) height * 2.) * abRange;

			for(int x = 0; x < width; x++) {
				const float a = (x / ( float) width * 2. - 1.) * abRange;
				pixels.setPixelColour(x, y, LabColor(color.L, a, b).getSRGBColor());
			}
		}
	}

	g.setOpacity(1.0);
	g.drawImageTransformed(colors,
		RectanglePlacement(RectanglePlacement::stretchToFit)
			.getTransformToFit(
				colors.getBounds().toFloat(), getLocalBounds().reduced(edge).toFloat()
			),
		false);
}

void LabColorSpaceView::resized() {
	colors = Image::null;
	updateMarker();
}

void LabColorSpaceView::mouseDown(const MouseEvent& e) {
	mouseDrag(e);
}

void LabColorSpaceView::mouseDrag(const MouseEvent& e) {
	const float a = ((e.x - edge) / (float) (getWidth() - edge * 2) * 2. - 1.) * abRange;
	const float b = (1. - 2. * (e.y - edge) / (float) (getHeight() - edge * 2)) * abRange;
	listener.updateAB(a, b);
}

void LabColorSpaceView::onLChanged() {
	colors = Image::null;
	repaint();
	updateMarker();
}

void LabColorSpaceView::onABChanged() {
	updateMarker();
}


void LabColorSpaceView::updateMarker() {
	marker.setBounds(
		roundToInt((getWidth() - edge * 2) * (color.a/abRange * .5 + .5)),
		roundToInt((getHeight() - edge * 2) * (.5 - color.b/abRange * .5)),
		edge * 2, edge * 2
	);
}

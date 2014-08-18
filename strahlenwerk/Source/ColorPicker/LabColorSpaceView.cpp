#include "LabColorSpaceView.h"

const float LabColorSpaceView::abRange = 110.;

LabColorSpaceView::LabColorSpaceView(const LabColor& color_) :
	edge(4),
	color(color_)
{
	color.addListenerForLab(this);

	marker.setSize(2 * edge, 2 * edge);
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
				pixels.setPixelColour(x, y, LabColor(color.L.getValue(), a, b).getSRGBColor());
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
	updateMarkerPosition();
}

void LabColorSpaceView::mouseDown(const MouseEvent& e) {
	setMouseCursor(MouseCursor::NoCursor);
	mouseDrag(e);
}

void LabColorSpaceView::mouseDrag(const MouseEvent& e) {
	// manual maker update
	marker.setCentrePosition(e.x, e.y);

	const float a = ((e.x - edge) / (float) (getWidth() - edge * 2) * 2. - 1.) * abRange;
	const float b = (1. - 2. * (e.y - edge) / (float) (getHeight() - edge * 2)) * abRange;
	color.a = a;
	color.b = b;
}

void LabColorSpaceView::mouseUp(const MouseEvent& e) {
	setMouseCursor(MouseCursor::CrosshairCursor);
}

void LabColorSpaceView::updateMarkerPosition() {
	marker.setCentrePosition(
			roundFloatToInt((getWidth())  * ( float(color.a.getValue()) / abRange * .5 + .5)),
			roundFloatToInt((getHeight()) * (-float(color.b.getValue()) / abRange * .5 + .5))
			);
}

void LabColorSpaceView::valueChanged(Value& value) {
	if (value.refersToSameSourceAs(color.L)) {
		// L changed
		colors = Image::null;
	} else if (value.refersToSameSourceAs(color.a) || value.refersToSameSourceAs(color.b)) {
		// a or b changed
		updateMarkerPosition();
	}
}

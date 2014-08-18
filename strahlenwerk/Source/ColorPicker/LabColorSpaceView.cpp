#include "LabColorSpaceView.h"

const float LabColorSpaceView::abRange = 110.;

LabColorSpaceView::LabColorSpaceView(const LabColor& color_) :
	color(color_)
{
	color.addListenerForLab(this);

	const int markerSideLength = 12;
	marker.setSize(markerSideLength, markerSideLength);
	addAndMakeVisible(marker);

	setMouseCursor(MouseCursor::CrosshairCursor);
}

void LabColorSpaceView::paint(Graphics& g) {
	if(bufferedSpaceView.isNull()) {
		const int width = getWidth();
		const int height = getHeight();
		bufferedSpaceView = Image(Image::RGB, width, height, false);

		Image::BitmapData pixels(bufferedSpaceView, Image::BitmapData::writeOnly);
		for(int y = 0; y < height; y++) {
			const float b = -(y / float(height) * 2. - 1.) * abRange;

			for(int x = 0; x < width; x++) {
				const float a = (x / float(width) * 2. - 1.) * abRange;
				pixels.setPixelColour(x, y, LabColor(color.L.getValue(), a, b).getSRGBColor());
			}
		}
	}

	g.setOpacity(1.0);
	g.drawImageTransformed(bufferedSpaceView,
		RectanglePlacement(RectanglePlacement::stretchToFit)
			.getTransformToFit(
				bufferedSpaceView.getBounds().toFloat(), getLocalBounds().toFloat()
			),
		false);
}

void LabColorSpaceView::resized() {
	bufferedSpaceView = Image::null;
	updateMarkerPosition();
}

void LabColorSpaceView::mouseDown(const MouseEvent& e) {
	setMouseCursor(MouseCursor::NoCursor);
	mouseDrag(e);
}

void LabColorSpaceView::mouseDrag(const MouseEvent& e) {
	const float x = e.position.x;
	const float y = e.position.y;
	const float a =  (x / float(getWidth())  * 2. - 1.) * abRange;
	const float b = -(y / float(getHeight()) * 2. - 1.) * abRange;
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
		bufferedSpaceView = Image::null;
	} else if (value.refersToSameSourceAs(color.a) || value.refersToSameSourceAs(color.b)) {
		// a or b changed
		updateMarkerPosition();
	}
}

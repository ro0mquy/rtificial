#include "LabColorSpaceView.h"

LabColorSpaceView::LabColorSpaceView() :
	edge(4),
	L(50.)
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
		const auto black = Colour(0., 0., 0.);
		for(int y = 0; y < height; y++) {
			const float b = (1. - y / ( float) height * 2.) * 120.;

			for(int x = 0; x < width; x++) {
				const float a = (x / ( float) width * 2. - 1.) * 120.;
				const auto rgb = XYZ2RGB(Lab2XYZ(Vector3D<float>(L, a, b)));
				if(rgb.x < 0. || rgb.x > 1. || rgb.y < 0. || rgb.y > 1. || rgb.z < 0. || rgb.z > 1.) {
					pixels.setPixelColour(x, y, black);
				} else {
					const auto srgb = RGB2SRGB(rgb);
					pixels.setPixelColour(x, y, Colour(srgb.x * 255, srgb.y * 255, srgb.z * 255));
				}
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
	a = ((e.x - edge) / (float) (getWidth() - edge * 2) * 2. - 1.) * 120.;
	b = (1. - 2. * (e.y - edge) / (float) (getHeight() - edge * 2)) * 120.;
	updateMarker();
}

void LabColorSpaceView::setL(float _L) {
	L = _L;
	colors = Image::null;
	repaint();
	updateMarker();
}

void LabColorSpaceView::updateMarker() {
	marker.setBounds(
		roundToInt((getWidth() - edge * 2) * (a/240. + .5)),
		roundToInt((getHeight() - edge * 2) * (.5 - b/240.)),
		edge * 2, edge * 2
	);
}

Vector3D<float> LabColorSpaceView::Lab2XYZ(Vector3D<float> Lab) noexcept {
	const float fy = (Lab.x + 16.)/116.;
	const float fz = fy - Lab.z/200.;
	const float fx = Lab.y/500. + fy;

	const float epsilon = 216./24389.;
	const float kappa = 24389./27.;

	const float fx3 = fx * fx * fx;
	const float fy3 = fy * fy * fy;
	const float fz3 = fz * fz * fz;

	const float fx_l = (116. * fx - 16.) / kappa;
	const float fy_l = Lab.x / kappa;
	const float fz_l = (116. * fz - 16.) / kappa;

	const float x = fx3 <= epsilon ? fx_l : fx3;
	const float y = Lab.x <= kappa * epsilon ? fy_l : fy3;
	const float z = fz3 <= epsilon ? fz_l : fz3;

	return Vector3D<float>(
		x * 0.95047,
		y,
		z * 1.08883
	);
}

Vector3D<float> LabColorSpaceView::XYZ2RGB(Vector3D<float> XYZ) noexcept {
	return Vector3D<float>(
		  3.2404542 * XYZ.x + -1.5371385 * XYZ.y + -0.4985314 * XYZ.z,
		 -0.9692660 * XYZ.x +  1.8760108 * XYZ.y +  0.0415560 * XYZ.z,
		  0.0556434 * XYZ.x + -0.2040259 * XYZ.y +  1.0572252 * XYZ.z
	);
}

Vector3D<float> LabColorSpaceView::RGB2SRGB(Vector3D<float> RGB) noexcept {
	const float a = .055;
	return Vector3D<float>(
		RGB.x <= 0.0031308f ? 12.92 * RGB.x : (1. + a) * pow(RGB.x, 1./2.4) - a,
		RGB.y <= 0.0031308f ? 12.92 * RGB.y : (1. + a) * pow(RGB.y, 1./2.4) - a,
		RGB.z <= 0.0031308f ? 12.92 * RGB.z : (1. + a) * pow(RGB.z, 1./2.4) - a
	);
}

#include "LabColor.h"

const float LabColor::epsilon = 216./24389.;
const float LabColor::kappa = 24389./27.;
const Vector3D<float> LabColor::xyzR = Vector3D<float>(0.95047, 1., 1.08883);

LabColor::LabColor(float L, float a, float b) :
	L(L), a(a), b(b)
{
}

LabColor::LabColor(Vector3D<float> Lab) : LabColor(Lab.x, Lab.y, Lab.z)
{
}

LabColor::LabColor(Colour linearRGB) : LabColor(XYZ2Lab(RGB2XYZ(fromColour(linearRGB)))) {
}

LabColor::LabColor(const LabColor& other) :
	L(other.L),
	a(other.a),
	b(other.b)
{
}

void LabColor::setColor(const LabColor& other) {
	L = other.L.getValue();
	a = other.a.getValue();
	b = other.b.getValue();
}

LabColor& LabColor::operator=(const LabColor& other) {
	setColor(other);
	return *this;
}

void LabColor::addListenerForLab(Value::Listener* listener) {
	L.addListener(listener);
	a.addListener(listener);
	b.addListener(listener);
}

Colour LabColor::getSRGBColor() const {
	return toColour(RGB2SRGB(invalidToBlack(XYZ2RGB(Lab2XYZ(Vector3D<float>(L.getValue(), a.getValue(), b.getValue()))))));
}

Colour LabColor::getLinearRGBColor() const {
	return toColour(invalidToBlack(XYZ2RGB(Lab2XYZ(Vector3D<float>(L.getValue(), a.getValue(), b.getValue())))));
}

Vector3D<float> LabColor::Lab2XYZ(Vector3D<float> Lab) noexcept {
	const float fy = (Lab.x + 16.)/116.;
	const float fz = fy - Lab.z/200.;
	const float fx = Lab.y/500. + fy;


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
		x * xyzR.x,
		y * xyzR.y,
		z * xyzR.z
	);
}

Vector3D<float> LabColor::XYZ2RGB(Vector3D<float> XYZ) noexcept {
	return Vector3D<float>(
		  3.2404542 * XYZ.x + -1.5371385 * XYZ.y + -0.4985314 * XYZ.z,
		 -0.9692660 * XYZ.x +  1.8760108 * XYZ.y +  0.0415560 * XYZ.z,
		  0.0556434 * XYZ.x + -0.2040259 * XYZ.y +  1.0572252 * XYZ.z
	);
}

Vector3D<float> LabColor::XYZ2Lab(Vector3D<float> XYZ) noexcept {
	const float xr = XYZ.x/xyzR.x;
	const float yr = XYZ.y/xyzR.y;
	const float zr = XYZ.z/xyzR.z;

	const float fx = xr <= epsilon ? (kappa * xr + 16.)/116. : pow(xr, 1./3.);
	const float fy = yr <= epsilon ? (kappa * yr + 16.)/116. : pow(yr, 1./3.);
	const float fz = zr <= epsilon ? (kappa * zr + 16.)/116. : pow(zr, 1./3.);

	return Vector3D<float>(
		116. * fy - 16.,
		500. * (fx - fy),
		200. * (fy - fz)
	);
}

Vector3D<float> LabColor::RGB2XYZ(Vector3D<float> RGB) noexcept {
	return Vector3D<float>(
		 0.4124564 * RGB.x + 0.3575761 * RGB.y + 0.1804375 * RGB.z,
		 0.2126729 * RGB.x + 0.7151522 * RGB.y + 0.0721750 * RGB.z,
		 0.0193339 * RGB.x + 0.1191920 * RGB.y + 0.9503041 * RGB.z
	);
}

Vector3D<float> LabColor::RGB2SRGB(Vector3D<float> RGB) noexcept {
	const float a = .055;
	return Vector3D<float>(
		RGB.x <= 0.0031308f ? 12.92 * RGB.x : (1. + a) * pow(RGB.x, 1./2.4) - a,
		RGB.y <= 0.0031308f ? 12.92 * RGB.y : (1. + a) * pow(RGB.y, 1./2.4) - a,
		RGB.z <= 0.0031308f ? 12.92 * RGB.z : (1. + a) * pow(RGB.z, 1./2.4) - a
	);
}

Vector3D<float> LabColor::invalidToBlack(Vector3D<float> RGB) noexcept {
	if(RGB.x < 0. || RGB.x > 1. || RGB.y < 0. || RGB.y > 1. || RGB.z < 0. || RGB.z > 1.) {
		return Vector3D<float>(0., 0., 0.);
	} else {
		return RGB;
	}
}
Colour LabColor::toColour(Vector3D<float> color) noexcept {
	return Colour(color.x * 255, color.y * 255, color.z * 255);
}

Vector3D<float> LabColor::fromColour(Colour color) noexcept {
	return Vector3D<float>(color.getFloatRed(), color.getFloatGreen(), color.getFloatBlue());
}

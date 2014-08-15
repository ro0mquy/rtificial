#include "LabColor.h"

LabColor::LabColor(float L, float a, float b) :
	L(L), a(a), b(b)
{
}

Colour LabColor::getSRGBColor() const {
	const auto srgb = Lab2SRGB(Vector3D<float>(L, a, b));
	return Colour(srgb.x * 255, srgb.y * 255, srgb.z * 255);
}

Vector3D<float> LabColor::Lab2XYZ(Vector3D<float> Lab) noexcept {
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

Vector3D<float> LabColor::XYZ2RGB(Vector3D<float> XYZ) noexcept {
	return Vector3D<float>(
		  3.2404542 * XYZ.x + -1.5371385 * XYZ.y + -0.4985314 * XYZ.z,
		 -0.9692660 * XYZ.x +  1.8760108 * XYZ.y +  0.0415560 * XYZ.z,
		  0.0556434 * XYZ.x + -0.2040259 * XYZ.y +  1.0572252 * XYZ.z
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

Vector3D<float> LabColor::Lab2SRGB(Vector3D<float> Lab) noexcept {
	const auto rgb = XYZ2RGB(Lab2XYZ(Vector3D<float>(Lab.x, Lab.y, Lab.z)));
	if(rgb.x < 0. || rgb.x > 1. || rgb.y < 0. || rgb.y > 1. || rgb.z < 0. || rgb.z > 1.) {
		return Vector3D<float>(0., 0., 0.);
	} else {
		return RGB2SRGB(rgb);
	}
}

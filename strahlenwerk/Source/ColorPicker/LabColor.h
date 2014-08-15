#ifndef LABCOLOR_H
#define LABCOLOR_H

#include <juce>

class LabColor{
	public:
		LabColor(float L, float a, float b);
		Colour getSRGBColor() const;

		float L, a, b;

	private:
		static Vector3D<float> Lab2XYZ(Vector3D<float> Lab) noexcept;
		static Vector3D<float> XYZ2RGB(Vector3D<float> XYZ) noexcept;
		static Vector3D<float> RGB2SRGB(Vector3D<float> RGB) noexcept;
		static Vector3D<float> Lab2SRGB(Vector3D<float> Lab) noexcept;
};

#endif

#ifndef LABCOLOR_H
#define LABCOLOR_H

#include <juce>

class LabColor{
	public:
		LabColor(float L, float a, float b);
		LabColor(Vector3D<float> Lab);
		LabColor(Colour linearRGB);
		LabColor(const LabColor& other);

		void setColor(const LabColor& other);
		LabColor& operator=(const LabColor& other);

		void addListenerForLab(Value::Listener* listener);

		Colour getSRGBColor() const;
		Colour getLinearRGBColor() const;

		Value L, a, b;

	private:
		static Vector3D<float> Lab2XYZ(Vector3D<float> Lab) noexcept;
		static Vector3D<float> XYZ2RGB(Vector3D<float> XYZ) noexcept;
		static Vector3D<float> XYZ2Lab(Vector3D<float> XYZ) noexcept;
		static Vector3D<float> RGB2XYZ(Vector3D<float> RGB) noexcept;
		static Vector3D<float> RGB2SRGB(Vector3D<float> RGB) noexcept;
		static Vector3D<float> invalidToBlack(Vector3D<float> RGB) noexcept;
		static Colour toColour(Vector3D<float> color) noexcept;
		static Vector3D<float> fromColour(Colour color) noexcept;

		static const float epsilon;
		static const float kappa;
		static const Vector3D<float> xyzR;
};

#endif

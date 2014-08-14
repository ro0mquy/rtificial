#ifndef LABCOLORPICKER_H
#define LABCOLORPICKER_H

#include <juce>

class LabColorPicker : public Component {
	public:
		void paint(Graphics& g) override;
		void resized() override;

	private:
		static Vector3D<float> Lab2XYZ(Vector3D<float> Lab) noexcept;
		static Vector3D<float> XYZ2RGB(Vector3D<float> XYZ) noexcept;
		static Vector3D<float> RGB2SRGB(Vector3D<float> RGB) noexcept;

		Image colors;
};

#endif

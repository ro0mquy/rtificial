#ifndef LABCOLORPICKER_H
#define LABCOLORPICKER_H

#include <juce>

#include "ColorSpaceMarker.h"

class LabColorPicker : public Component {
	public:
		LabColorPicker();

		void paint(Graphics& g) override;
		void resized() override;
		void mouseDown(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;

	private:
		void updateMarker();

		static Vector3D<float> Lab2XYZ(Vector3D<float> Lab) noexcept;
		static Vector3D<float> XYZ2RGB(Vector3D<float> XYZ) noexcept;
		static Vector3D<float> RGB2SRGB(Vector3D<float> RGB) noexcept;

		Image colors;
		ColorSpaceMarker marker;
		int edge;

		float L;
		float a;
		float b;
};

#endif

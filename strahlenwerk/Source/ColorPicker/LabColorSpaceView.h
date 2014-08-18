#ifndef LABCOLORSPACEVIEW_H
#define LABCOLORSPACEVIEW_H

#include <juce>

#include "ColorSpaceMarker.h"
#include "LabColor.h"

class LabColorSpaceView :
	public Component,
	private Value::Listener
{
	public:
		LabColorSpaceView(const LabColor& color_);

		void paint(Graphics& g) override;
		void resized() override;
		void mouseDown(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void mouseUp(const MouseEvent& e) override;
		void valueChanged(Value& value) override;

	private:
		void updateMarkerPosition();

		Image colors;
		int edge;
		LabColor color;
		ColorSpaceMarker marker;

		static const float abRange;
};

#endif

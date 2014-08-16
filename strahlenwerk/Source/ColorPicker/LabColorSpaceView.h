#ifndef LABCOLORSPACEVIEW_H
#define LABCOLORSPACEVIEW_H

#include <juce>

#include "ColorSpaceMarker.h"
#include "ColorChangedListener.h"
#include "LabColor.h"

class LabColorSpaceView : public Component {
	public:
		LabColorSpaceView(ColorChangedListener& listener, const LabColor& color);

		void paint(Graphics& g) override;
		void resized() override;
		void mouseDown(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void onLChanged();
		void onABChanged();
		void onColorChanged();


	private:
		void updateMarker();

		Image colors;
		ColorSpaceMarker marker;
		int edge;

		const LabColor& color;

		ColorChangedListener& listener;

		static const float abRange;
};

#endif

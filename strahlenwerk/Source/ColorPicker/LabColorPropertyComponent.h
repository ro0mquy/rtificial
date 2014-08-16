#ifndef LABCOLORPROPERTYCOMPONENT_H
#define LABCOLORPROPERTYCOMPONENT_H

#include <juce>

#include "LabColorPicker.h"

class LabColorPropertyComponent : public PropertyComponent {
	public:
		LabColorPropertyComponent(const String& name);

		void refresh() override;
		void paint(Graphics& g) override;

		void mouseUp(const MouseEvent &event) override;

	private:
		LabColorPicker picker;
		Component dummy;
};
#endif

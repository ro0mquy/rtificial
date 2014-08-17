#ifndef LABCOLORPROPERTYCOMPONENT_H
#define LABCOLORPROPERTYCOMPONENT_H

#include <juce>

#include "LabColor.h"

class LabColorPropertyComponent :
	public PropertyComponent,
	private Value::Listener
{
	public:
		LabColorPropertyComponent(const String& name);

		void refresh() override;
		void paint(Graphics& g) override;

		void mouseUp(const MouseEvent &event) override;
		void valueChanged(Value& value) override;

	private:
		LabColor color;
		Component dummy;
};
#endif

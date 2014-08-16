#ifndef LABCOLORPROPERTYCOMPONENT_H
#define LABCOLORPROPERTYCOMPONENT_H

#include <juce>

#include "LabColorPicker.h"

class LabColorPropertyComponent :
	public PropertyComponent,
	private ChangeListener
{
	public:
		LabColorPropertyComponent(const String& name);

		void refresh() override;
		void paint(Graphics& g) override;

		void mouseUp(const MouseEvent &event) override;
		void changeListenerCallback(ChangeBroadcaster* source);

	private:
		LabColorPicker* picker;
		LabColor color;
		Component dummy;
};
#endif

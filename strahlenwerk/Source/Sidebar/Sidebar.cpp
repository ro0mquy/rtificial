#include "Sidebar.h"

Sidebar::Sidebar() :
	TabbedComponent(TabbedButtonBar::TabsAtTop)
{
	addTab("Inspector", findColour(Sidebar::tabBackgroundColourId), &inspectorComponent, false);
}

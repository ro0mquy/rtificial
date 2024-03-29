#include "Sidebar.h"

Sidebar::Sidebar() :
	TabbedComponent(TabbedButtonBar::TabsAtTop)
{
	setOutline(0);
	addTab("Inspector", findColour(Sidebar::tabBackgroundColourId), &inspectorComponent, false);
	addTab("Info Log", findColour(Sidebar::tabBackgroundColourId), &infoLogComponent, false);
	addTab("Scopes", findColour(Sidebar::tabBackgroundColourId), &scopesComponent, false);
	addTab("Performance", findColour(Sidebar::tabBackgroundColourId), &performanceComponent, false);
}

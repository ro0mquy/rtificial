#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <juce>
#include "InspectorComponent.h"
#include "InfoLogComponent.h"
#include "ScopesComponent.h"
#include "PerformanceComponent.h"

class Sidebar :
	public TabbedComponent
{
	public:
		Sidebar();

		enum ColourIds {
			tabBackgroundColourId = 0x23c0301,
		};

	private:
		InspectorComponent inspectorComponent;
		InfoLogComponent infoLogComponent;
		ScopesComponent scopesComponent;
		PerformanceComponent performanceComponent;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidebar)
};

#endif // SIDEBAR_H

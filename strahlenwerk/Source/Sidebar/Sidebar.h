#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <juce>

class Sidebar :
	public PropertyPanel
{
	public:
		Sidebar();

	private:
		ValueTree uniformStatesArray;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidebar)
};

#endif // SIDEBAR_H

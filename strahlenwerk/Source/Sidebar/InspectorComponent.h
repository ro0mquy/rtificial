#ifndef INSPECTORCOMPONENT_H
#define INSPECTORCOMPONENT_H

#include <juce>

class Selection;

class InspectorComponent :
	public Component,
	public ChangeListener
{
	public:
		InspectorComponent();
		~InspectorComponent();

		void changeListenerCallback(ChangeBroadcaster* source);

	private:
		Selection& selection;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorComponent)
};

#endif // INSPECTORCOMPONENT_H

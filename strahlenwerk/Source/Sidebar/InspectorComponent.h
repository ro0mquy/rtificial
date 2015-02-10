#ifndef INSPECTORCOMPONENT_H
#define INSPECTORCOMPONENT_H

#include <juce>
#include <Sidebar/SequenceBackgroundComponent.h>

class Selection;

class InspectorComponent :
	public Component,
	public ChangeListener
{
	public:
		InspectorComponent();
		~InspectorComponent();

		void changeListenerCallback(ChangeBroadcaster* source);

		void resized() override;
		void paint(Graphics& g) override;

	private:
		Selection& selection;
		ValueTree singleSelectedTree;
		ScopedPointer<SequenceBackgroundComponent> sequencePreview;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorComponent)
};

#endif // INSPECTORCOMPONENT_H

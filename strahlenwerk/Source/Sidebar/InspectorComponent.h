#ifndef INSPECTORCOMPONENT_H
#define INSPECTORCOMPONENT_H

#include <juce>
#include <Sidebar/SequenceBackgroundComponent.h>
#include <Sidebar/ValueEditorPropertyComponent.h>

class Selection;

class InspectorComponent :
	public Component,
	public ChangeListener
{
	public:
		InspectorComponent();
		~InspectorComponent();

		void changeListenerCallback(ChangeBroadcaster* source) override;

		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds {
			textColourId = 0x2c34a01,
		};

	private:
		Selection& selection;
		AudioManager& audioManager;

		ValueTree singleSelectedTree;
		ScopedPointer<SequenceBackgroundComponent> sequencePreview;
		ScopedPointer<ValueEditorPropertyComponent> keyframeValueEditor;

		void initalizeSequenceEditing();
		void updateSequenceEditor();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorComponent)
};

#endif // INSPECTORCOMPONENT_H

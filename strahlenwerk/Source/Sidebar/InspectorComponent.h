#ifndef INSPECTORCOMPONENT_H
#define INSPECTORCOMPONENT_H

#include <juce>
#include <Sidebar/SequenceBackgroundComponent.h>
#include <Sidebar/ValueEditorPropertyComponent.h>

class TimelineData;
class Selection;
class AudioManager;

class InspectorComponent :
	public Component,
	private ValueTree::Listener,
	private ChangeListener,
	private ApplicationCommandManagerListener
{
	public:
		InspectorComponent();
		~InspectorComponent();

		void changeListenerCallback(ChangeBroadcaster* source) override;

		void resized() override;
		void paint(Graphics& g) override;

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

		void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
		void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
		void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
		void valueTreeChildOrderChanged(ValueTree& parentTree, int oldIndex, int newIndex) override;
		void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

		enum ColourIds {
			textColourId = 0x2c34a01,
		};

		enum CommandIDs {
			toggleLoop = 0x47e15a00,
		};

	private:
		TimelineData& data;
		Selection& selection;
		AudioManager& audioManager;

		ValueTree singleSelectedTree;
		ScopedPointer<SequenceBackgroundComponent> sequencePreview;
		ScopedPointer<ValueEditorPropertyComponent> keyframeValueEditor;

		void initalizeSequenceEditing();
		void updateSequenceEditor();

		void checkIfNeedToLoop();
		void performToggleLoop();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorComponent)
};

#endif // INSPECTORCOMPONENT_H

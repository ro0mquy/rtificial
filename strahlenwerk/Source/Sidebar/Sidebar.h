#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <juce>

class ValueEditorPropertyComponent;
class TimelineData;

class Sidebar :
	public PropertyPanel,
	private ValueTree::Listener,
	private ChangeListener,
	private AsyncUpdater
{
	public:
		Sidebar();
		~Sidebar();

		void addEditorComponentToArray(ValueTree uniform);
		void reAddAllProperties();
		void updateEditorValueData(ValueTree uniform);
		void updateAllEditorValueDatas();

		void handleAsyncUpdate() override;
		void changeListenerCallback(ChangeBroadcaster* source) override;

		// ValueTree::Listener callbacks
        void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
        void valueTreeChildOrderChanged(ValueTree& parentTree) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;
		void valueTreeRedirected(ValueTree& treeWhoWasRedirected) override;

	private:
		Array<ValueEditorPropertyComponent*> editorComponentsArray;
		TimelineData& data;

		bool constructionCompleted = false;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidebar)
};

#endif // SIDEBAR_H

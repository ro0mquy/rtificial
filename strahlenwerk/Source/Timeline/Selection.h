#ifndef SELECTION_H
#define SELECTION_H

#include <juce>

class TimelineData;
class AudioManager;

class Selection :
	public ChangeBroadcaster,
	private ChangeListener,
	private ApplicationCommandManagerListener
{
	public:
		Selection(TimelineData& data_);
		~Selection();

		int size();
		ValueTree* operator[](const int index);
		void set(ValueTree tree);
		void add(ValueTree tree);
		void clear();
		bool contains(ValueTree treeToCheckFor);

		void changeListenerCallback(ChangeBroadcaster* source) override;
		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

		enum CommandIDs {
			toggleLoop = 0x47e15a00,
		};

	private:
		OwnedArray<ValueTree> selectedTrees;

		TimelineData& data;
		AudioManager& audioManager;

		void checkIfNeedToLoop();
		void performToggleLoop();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Selection)
};

#endif // SELECTION_H

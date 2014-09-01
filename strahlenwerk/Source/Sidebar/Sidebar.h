#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <juce>

class TimelineData;

class Sidebar :
	public PropertyPanel,
	private ValueTree::Listener
{
	public:
		Sidebar();

		void updateProperties();

		// ValueTree::Listener callbacks
        void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
        void valueTreeChildOrderChanged(ValueTree& parentTree) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

	private:
		TimelineData& data;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sidebar)
};

#endif // SIDEBAR_H

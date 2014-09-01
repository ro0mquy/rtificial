#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <juce>

class TimelineData;

class Interpolator :
	public ValueTree::Listener
{
	public:
		Interpolator(TimelineData& data_);

		ValueTree getUniformStatesArray();
		void updateUniformState(ValueTree uniformData);
		void updateUniformState(const var& name);
		void updateAllUniformStates();

		// ValueTree::Listener callbacks
        void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;
        void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) override;
        void valueTreeChildOrderChanged(ValueTree& parentTree) override;
        void valueTreeParentChanged(ValueTree& treeWhoseParentHasChanged) override;

	private:
		TimelineData& data;
		ValueTree uniformStatesArray;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

#endif // INTERPOLATOR_H

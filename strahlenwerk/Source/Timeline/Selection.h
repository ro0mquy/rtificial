#ifndef SELECTION_H
#define SELECTION_H

#include <juce>

class Selection :
	public ChangeBroadcaster
{
	public:
		Selection();

		int size();
		ValueTree* operator[](const int index);
		void set(ValueTree tree);
		void add(ValueTree tree);
		void clear();
		bool contains(ValueTree treeToCheckFor);

	private:
		OwnedArray<ValueTree> selectedTrees;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Selection)
};

#endif // SELECTION_H

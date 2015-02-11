#include "Selection.h"

Selection::Selection() {
}

int Selection::size() {
	return selectedTrees.size();
}

ValueTree* Selection::operator[](const int index) {
	return selectedTrees[index];
}

void Selection::set(ValueTree tree) {
	if (selectedTrees.size() == 1 && contains(tree)) {
		// tree is already the only selected value tree
		return;
	}
	selectedTrees.clear();
	selectedTrees.add(new ValueTree(tree));
	sendChangeMessage();
}

void Selection::add(ValueTree tree) {
	if (contains(tree)) {
		return;
	}
	selectedTrees.add(new ValueTree(tree));
	sendChangeMessage();
}

void Selection::clear() {
	selectedTrees.clear();
	sendChangeMessage();
}

bool Selection::contains(ValueTree treeToCheckFor) {
	const int numTrees = selectedTrees.size();
	for (int i = 0; i < numTrees; i++) {
		if (*selectedTrees.getUnchecked(i) == treeToCheckFor) {
			return true;
		}
	}
	return false;
}

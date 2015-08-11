#include "Selection.h"

#include <Timeline/TimelineData.h>
#include <MainWindow.h>
#include <AudioManager.h>
#include <StrahlenwerkApplication.h>
#include <PropertyNames.h>

Selection::Selection(TimelineData& data_) :
	data(data_),
	audioManager(AudioManager::getAudioManager())
{
	MainWindow::getApplicationCommandManager().addListener(this);

	PropertySet& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool loopEnabled = properties.getBoolValue(PropertyNames::LoopEnabled);
	if (loopEnabled) {
		audioManager.addChangeListener(this);
	}
}

Selection::~Selection() {
	audioManager.removeChangeListener(this);
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

void Selection::remove(ValueTree tree) {
	if (!contains(tree)) {
		return;
	}
	selectedTrees.remove(indexOf(tree));
	sendChangeMessage();
}

void Selection::clear() {
	selectedTrees.clear();
	sendChangeMessage();
}

int Selection::indexOf(ValueTree treeToCheckFor) {
	const int numTrees = selectedTrees.size();
	for (int i = 0; i < numTrees; i++) {
		if (*selectedTrees.getUnchecked(i) == treeToCheckFor) {
			return i;
		}
	}
	return -1;
}

bool Selection::contains(ValueTree treeToCheckFor) {
	return indexOf(treeToCheckFor) != -1;
}


void Selection::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// time changed
	checkIfNeedToLoop();
}

void Selection::checkIfNeedToLoop() {
	if (size() == 1) {
		// loop selection
		int start;
		int duration;
		if (data.isSequence(*selectedTrees[0])) {
			start = data.getAbsoluteStartForSequence(*selectedTrees[0]);
			duration = data.getSequenceDuration(*selectedTrees[0]);
		} else if (data.isKeyframe(*selectedTrees[0])) {
			ValueTree parentSequence = data.getKeyframeParentSequence(*selectedTrees[0]);
			start = data.getAbsoluteStartForSequence(parentSequence);
			duration = data.getSequenceDuration(parentSequence);
		} else if (data.isScene(*selectedTrees[0])) {
			start = data.getSceneStart(*selectedTrees[0]);
			duration = data.getSceneDuration(*selectedTrees[0]);
		} else {
			return;
		}
		const int end = start + duration;
		const int currentTime = audioManager.getTime();
		const int deltaTime = currentTime - end;

		if (deltaTime > 0) {
			audioManager.setTime(start + deltaTime % duration);
		}
	}
}

void Selection::performToggleLoop() {
	PropertySet& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::LoopEnabled);
	if (previous == true) {
		// disable looping
		audioManager.removeChangeListener(this);
	} else {
		// enable looping
		audioManager.addChangeListener(this);
	}
	properties.setValue(PropertyNames::LoopEnabled, !previous);
}

void Selection::performDeleteSelection() {
	const int numSelection = size();
	if (numSelection == 1) {
		ValueTree singleElement = *selectedTrees.getUnchecked(0);
		if (data.isScene(singleElement)) {
			data.getUndoManager().beginNewTransaction("Delete Scene");
			data.removeScene(singleElement);
		} else if (data.isSequence(singleElement)) {
			data.getUndoManager().beginNewTransaction("Delete Sequence");
			data.removeSequence(singleElement);
		} else if (data.isKeyframe(singleElement)) {
			data.getUndoManager().beginNewTransaction("Delete Keyframe");
			data.removeKeyframe(singleElement);
		} else {
			jassertfalse;
		}
	} else {
		data.getUndoManager().beginNewTransaction("Delete Selection");
		for (int i = 0; i < numSelection; i++) {
			ValueTree element = *selectedTrees[i];
			if (data.isScene(element)) {
				data.removeScene(element);
			} else if (data.isSequence(element)) {
				data.removeSequence(element);
			} else if (data.isKeyframe(element)) {
				data.removeKeyframe(element);
			} else {
				jassertfalse;
			}
		}
	}
	clear();
}

void Selection::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case Selection::toggleLoop:
			performToggleLoop();
			break;
		case Selection::deleteSelection:
			performDeleteSelection();
			break;
	}
}

void Selection::applicationCommandListChanged() {
}

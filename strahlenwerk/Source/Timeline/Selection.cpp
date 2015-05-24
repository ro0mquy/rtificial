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


void Selection::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// time changed
	checkIfNeedToLoop();
}

void Selection::checkIfNeedToLoop() {
	if (size() == 1 && data.isSequence(*selectedTrees[0])) {
		// loop selection
		const int sequenceStart = data.getAbsoluteStartForSequence(*selectedTrees[0]);
		const int sequenceDuration = data.getSequenceDuration(*selectedTrees[0]);
		const int sequenceEnd = sequenceStart + sequenceDuration;
		const int currentTime = audioManager.getTime();

		if (currentTime > sequenceEnd) {
			audioManager.setTime(sequenceStart);
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

void Selection::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case Selection::toggleLoop:
			performToggleLoop();
			break;
	}
}

void Selection::applicationCommandListChanged() {
}

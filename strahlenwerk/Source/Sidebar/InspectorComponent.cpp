#include "InspectorComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <AudioManager.h>

InspectorComponent::InspectorComponent() :
	data(TimelineData::getTimelineData()),
	selection(data.getSelection()),
	audioManager(AudioManager::getAudioManager())
{
	data.addListenerToTree(this);
	selection.addChangeListener(this);
	audioManager.addChangeListener(this);
}

InspectorComponent::~InspectorComponent() {
	data.removeListenerFromTree(this);
	selection.removeChangeListener(this);
	audioManager.removeChangeListener(this);
}

void InspectorComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &audioManager) {
		updateSequenceEditor();
	} else if (source == &selection) {
		// selection changed
		const int selectionSize = selection.size();

		if (selectionSize == 0) {
			singleSelectedTree = ValueTree();
			sequencePreview = nullptr;
			keyframeValueEditor = nullptr;
		} else if (selectionSize == 1) {
			initalizeSequenceEditing();
		} else {
			singleSelectedTree = ValueTree();
			sequencePreview = nullptr;
			keyframeValueEditor = nullptr;
		}
		repaint();
	}
}

void InspectorComponent::resized() {
	if (sequencePreview != nullptr && keyframeValueEditor != nullptr) {
		const int scenesBarHeightHalf = 30 / 2;
		const int rowHeight = 20;
		const int padding = 30;
		Rectangle<int> previewRect(0, 0, getWidth(), scenesBarHeightHalf + rowHeight + 2 * padding);
		previewRect.reduce(padding, padding);

		sequencePreview->setBounds(previewRect);

		keyframeValueEditor->setBounds(previewRect.getX(), previewRect.getBottom() + padding, previewRect.getWidth(), keyframeValueEditor->getPreferredHeight());
	}
}

void InspectorComponent::paint(Graphics& g) {
	if (singleSelectedTree.hasType(treeId::sequence)) {
	} else {
		g.setColour(findColour(InspectorComponent::textColourId));
		g.setFont(g.getCurrentFont().withStyle(Font::FontStyleFlags::italic));
		g.drawSingleLineText("No Sequence Selected", getWidth() / 2, 50, Justification::horizontallyCentred);
	}
}

void InspectorComponent::initalizeSequenceEditing() {
	singleSelectedTree = *selection[0];
	sequencePreview = new SequenceBackgroundComponent(singleSelectedTree);

	TimelineData& data = TimelineData::getTimelineData();
	ValueTree uniform = data.getSequenceParentUniform(singleSelectedTree);
	const String name = data.getUniformName(uniform);

	Interpolator::UniformState uniformState = data.getInterpolator().getCurrentUniformState(uniform);
	ValueTree value = uniformState.first;
	const bool isOnKeyframe = uniformState.second;

	keyframeValueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(name, value);
	keyframeValueEditor->setEnabled(isOnKeyframe);

	resized();
	addAndMakeVisible(sequencePreview);
	addAndMakeVisible(keyframeValueEditor);
}

void InspectorComponent::updateSequenceEditor() {
	if (sequencePreview == nullptr || keyframeValueEditor == nullptr) {
		return;
	}

	TimelineData& data = TimelineData::getTimelineData();
	ValueTree uniform = data.getSequenceParentUniform(singleSelectedTree);

	Interpolator::UniformState uniformState = data.getInterpolator().getCurrentUniformState(uniform);
	ValueTree value = uniformState.first;
	const bool isOnKeyframe = uniformState.second;

	keyframeValueEditor->useValueData(value);
	keyframeValueEditor->setEnabled(isOnKeyframe);
}

// ValueTree::Listener callbacks
void InspectorComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (parentTree == singleSelectedTree) {
		// any of the sequence properties changed
		updateSequenceEditor();
	} else if (property == treeId::sceneStart) {
		if (singleSelectedTree.isValid() && parentTree == data.getScene(data.getSequenceSceneId(singleSelectedTree))) {
			// the scene this sequence belongs to has been moved
			updateSequenceEditor();
		}
	} else if ((parentTree.hasType(treeId::keyframe) && data.getKeyframeParentSequence(parentTree) == singleSelectedTree) || (parentTree.hasType(treeId::keyframeValue) && data.getKeyframeParentSequence(parentTree.getParent()) == singleSelectedTree)) {
		// a keyframe from the sequence has changed
		updateSequenceEditor();
	}
}

void InspectorComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& /*childWhichHasBeenAdded*/) {
	if (singleSelectedTree.isValid() && data.getKeyframesArray(singleSelectedTree) == parentTree) {
		// a keyframe was added
		updateSequenceEditor();
	}
}

void InspectorComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
	if (singleSelectedTree.isValid() && data.getKeyframesArray(singleSelectedTree) == parentTree) {
		// a keyframe was removed
		updateSequenceEditor();
	}
}

void InspectorComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void InspectorComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

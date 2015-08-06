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
		// time changed
		updateSequenceEditor();
	} else if (source == &selection) {
		// selection changed
		const int selectionSize = selection.size();

		singleSelectedTree = ValueTree();
		sequencePreview = nullptr;
		keyframeValueEditor = nullptr;

		if (selectionSize == 0) {
		} else if (selectionSize == 1) {
			singleSelectedTree = *selection[0];
			if (isEditingSequence()) {
				initalizeSequenceEditing();
			} else if (isEditingKeyframe()) {
				initalizeKeyframeEditing();
			} else {
			}
		} else {
		}
		repaint();
	}
}

void InspectorComponent::resized() {
	if ((isEditingSequence() || isEditingKeyframe()) && sequencePreview != nullptr && keyframeValueEditor != nullptr) {
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
	if (isEditingSequence()) {
	} else if (isEditingKeyframe()) {
	} else {
		g.setColour(findColour(InspectorComponent::textColourId));
		g.setFont(g.getCurrentFont().withStyle(Font::FontStyleFlags::italic));
		g.drawSingleLineText("No Sequence Selected", getWidth() / 2, 50, Justification::horizontallyCentred);
	}
}

void InspectorComponent::initalizeSequenceEditing() {
	sequencePreview = new SequenceBackgroundComponent(singleSelectedTree);

	ValueTree uniform = data.getSequenceParentUniform(singleSelectedTree);
	const String name = data.getUniformName(uniform);

	Interpolator::UniformState uniformState = data.getInterpolator().getCurrentUniformStateWithSequence(singleSelectedTree);
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

	Interpolator::UniformState uniformState = data.getInterpolator().getCurrentUniformStateWithSequence(singleSelectedTree);
	ValueTree value = uniformState.first;
	const bool isOnKeyframe = uniformState.second;

	keyframeValueEditor->useValueData(value);
	keyframeValueEditor->setEnabled(isOnKeyframe);
}

bool InspectorComponent::isEditingSequence() {
	return data.isSequence(singleSelectedTree);
}

void InspectorComponent::initalizeKeyframeEditing() {
	ValueTree parentSequence = data.getKeyframeParentSequence(singleSelectedTree);
	sequencePreview = new SequenceBackgroundComponent(parentSequence);

	ValueTree uniform = data.getSequenceParentUniform(parentSequence);
	const String name = data.getUniformName(uniform);
	ValueTree value = data.getKeyframeValue(singleSelectedTree);

	keyframeValueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(name, value);
	keyframeValueEditor->setEnabled(true);

	resized();
	addAndMakeVisible(sequencePreview);
	addAndMakeVisible(keyframeValueEditor);
}

bool InspectorComponent::isEditingKeyframe() {
	return data.isKeyframe(singleSelectedTree);
}

// ValueTree::Listener callbacks
void InspectorComponent::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) {
	if (isEditingSequence()) {
		if (parentTree == singleSelectedTree) {
			// any of the sequence properties changed
			updateSequenceEditor();
		} else if (property == treeId::sceneStart) {
			if (parentTree == data.getScene(data.getSequenceSceneId(singleSelectedTree))) {
				// the scene this sequence belongs to has been moved
				updateSequenceEditor();
			}
		} else if ((parentTree.hasType(treeId::keyframe) && data.getKeyframeParentSequence(parentTree) == singleSelectedTree) || (parentTree.hasType(treeId::keyframeValue) && data.getKeyframeParentSequence(parentTree.getParent()) == singleSelectedTree)) {
			// a keyframe from the sequence has changed
			updateSequenceEditor();
		}
	}
}

void InspectorComponent::valueTreeChildAdded(ValueTree& parentTree, ValueTree& /*childWhichHasBeenAdded*/) {
	if (isEditingSequence()) {
		if (data.getKeyframesArray(singleSelectedTree) == parentTree) {
			// a keyframe was added
			updateSequenceEditor();
		}
	}
}

void InspectorComponent::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
	if (isEditingSequence()) {
		if (data.getKeyframesArray(singleSelectedTree) == parentTree) {
			// a keyframe was removed
			updateSequenceEditor();
		}
	}
}

void InspectorComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void InspectorComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

#include "InspectorComponent.h"

#include <Timeline/TimelineData.h>
#include <Timeline/TreeIdentifiers.h>
#include <StrahlenwerkApplication.h>
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
		if (isEditingSequence()) {
			updateSequenceEditor();
		}
	} else if (source == &selection) {
		// selection changed
		const int selectionSize = selection.size();

		singleSelectedTree = ValueTree();
		sequencePreview = nullptr;
		scenePreview = nullptr;
		keyframeValueEditor = nullptr;
		propertyEditorPanel = nullptr;

		if (selectionSize == 0) {
		} else if (selectionSize == 1) {
			singleSelectedTree = *selection[0];
			if (isEditingSequence()) {
				initalizeSequenceEditing();
			} else if (isEditingKeyframe()) {
				initalizeKeyframeEditing();
			} else if (isEditingScene()) {
				initalizeSceneEditing();
			} else {
			}
		} else {
		}
		repaint();
	}
}

void InspectorComponent::resized() {
	if ((isEditingSequence() || isEditingKeyframe()) && sequencePreview != nullptr && propertyEditorPanel != nullptr) {
		const int scenesBarHeightHalf = 30 / 2;
		const int rowHeight = 20;
		const int padding = 30;

		Rectangle<int> boundsRect = getLocalBounds().reduced(padding);
		Rectangle<int> previewRect = boundsRect.removeFromTop(scenesBarHeightHalf + rowHeight);

		sequencePreview->setBounds(previewRect);

		propertyEditorPanel->setBounds(boundsRect.withTrimmedTop(padding));
	} else if (isEditingScene() && scenePreview != nullptr && propertyEditorPanel != nullptr) {
		const int scenesBarHeightHalf = 30 / 2;
		const int rowHeight = 20;
		const int padding = 30;

		Rectangle<int> boundsRect = getLocalBounds().reduced(padding);
		Rectangle<int> previewRect = boundsRect.removeFromTop(scenesBarHeightHalf + rowHeight);

		scenePreview->setBounds(previewRect);

		propertyEditorPanel->setBounds(boundsRect.withTrimmedTop(padding));
	}
}

void InspectorComponent::paint(Graphics& g) {
	if (isEditingSequence()) {
	} else if (isEditingKeyframe()) {
	} else if (isEditingScene()) {
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

	const String interpolationMethods[] = { "step", "linear", "ccrSpline" };
	const int numMethods = numElementsInArray(interpolationMethods);
	const StringArray interpolationMethodsStringArray(interpolationMethods, numMethods);
	const Array<var> interpolationMethodsVarArray(interpolationMethods, numMethods);
	Value sequenceInterpolation = data.getSequenceInterpolationAsValue(singleSelectedTree);
	PropertyComponent* interpolationEditor = new ChoicePropertyComponent(sequenceInterpolation, "interpolation method", interpolationMethodsStringArray, interpolationMethodsVarArray);

	propertyEditorPanel = new PropertyPanel;
	propertyEditorPanel->addProperties({{ static_cast<PropertyComponent*>(keyframeValueEditor), interpolationEditor }});

	resized();
	addAndMakeVisible(sequencePreview);
	addAndMakeVisible(propertyEditorPanel);
}

void InspectorComponent::updateSequenceEditor() {
	if (keyframeValueEditor == nullptr || propertyEditorPanel == nullptr) {
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

	PropertyComponent* valueEditor = ValueEditorPropertyComponent::newValueEditorPropertyComponent(name, value);
	valueEditor->setEnabled(true);

	/*
	Value keyframePosition = data.getKeyframePositionAsValue(singleSelectedTree);
	double sequenceDuration = data.getSequenceDuration(parentSequence);
	PropertyComponent* positionEditor = new SliderPropertyComponent(keyframePosition, "Position", 0., sequenceDuration, 1.);
	// */

	Value keyframeEaseToward = data.getKeyframeEaseTowardAsValue(singleSelectedTree);
	PropertyComponent* easeTowardEditor = new BooleanPropertyComponent(keyframeEaseToward, "ease toward", String::empty);
	easeTowardEditor->setColour(BooleanPropertyComponent::backgroundColourId, Colour(0x00000000));

	Value keyframeEaseAway = data.getKeyframeEaseAwayAsValue(singleSelectedTree);
	PropertyComponent* easeAwayEditor = new BooleanPropertyComponent(keyframeEaseAway, "ease away", String::empty);
	easeAwayEditor->setColour(BooleanPropertyComponent::backgroundColourId, Colour(0x00000000));

	propertyEditorPanel = new PropertyPanel;
	propertyEditorPanel->addProperties({{ valueEditor, /*positionEditor,*/ easeTowardEditor, easeAwayEditor }});

	resized();
	addAndMakeVisible(sequencePreview);
	addAndMakeVisible(propertyEditorPanel);
}

bool InspectorComponent::isEditingKeyframe() {
	return data.isKeyframe(singleSelectedTree);
}

void InspectorComponent::initalizeSceneEditing() {
	scenePreview = new SceneBackgroundComponent(singleSelectedTree);

	Value sceneShaderSource = data.getSceneShaderSourceAsValue(singleSelectedTree);
	const std::vector<File> allSceneShaderFiles = StrahlenwerkApplication::getInstance()->getProject().getLoader().listSceneFiles();
	StringArray sceneShaderStringArray;
	Array<var> sceneShaderVarArray;
	for (const File& shaderFile : allSceneShaderFiles) {
		const String shaderName = shaderFile.getFileNameWithoutExtension();
		sceneShaderStringArray.add(shaderName);
		sceneShaderVarArray.add(shaderName);
	}
	PropertyComponent* shaderSourceEditor = new ChoicePropertyComponent(sceneShaderSource, "shader name", sceneShaderStringArray, sceneShaderVarArray);

	Value sceneEnvironmentSource = data.getSceneEnvironmentSourceAsValue(singleSelectedTree);
	const std::vector<File> allEnvironmentFiles = StrahlenwerkApplication::getInstance()->getProject().getLoader().listEnvironmentFiles();
	StringArray environmentStringArray;
	Array<var> environmentVarArray;
	for (const File& environmentFile : allEnvironmentFiles) {
		const String shaderName = environmentFile.getFileNameWithoutExtension();
		environmentStringArray.add(shaderName);
		environmentVarArray.add(shaderName);
	}
	PropertyComponent* environmentSourceEditor = new ChoicePropertyComponent(sceneEnvironmentSource, "environment name", environmentStringArray, environmentVarArray);

	propertyEditorPanel = new PropertyPanel;
	propertyEditorPanel->addProperties({{ shaderSourceEditor, environmentSourceEditor }});

	resized();
	addAndMakeVisible(scenePreview);
	addAndMakeVisible(propertyEditorPanel);
}

bool InspectorComponent::isEditingScene() {
	return data.isScene(singleSelectedTree);
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

#ifndef INSPECTORSEQUENCECOMPONENT_H
#define INSPECTORSEQUENCECOMPONENT_H

#include <juce>
#include <Timeline/KeyframeComponent.h>

class TimelineData;
class ZoomFactor;
class AudioManager;

class InspectorSequenceComponent :
	public Component
{
	public:
		InspectorSequenceComponent();
		InspectorSequenceComponent(ValueTree sequenceData_);

		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;

		void setSequenceData(ValueTree sequenceData_);
		void addKeyframeComponent(ValueTree keyframeData);
		void addAllKeyframeComponents();
		AffineTransform getKeyframeTransform();

		ValueTree sequenceData;

	private:
		TimelineData& data;
		AudioManager& audioManager;
		OwnedArray<KeyframeComponent> keyframeComponentsArray;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorSequenceComponent)
};

#endif // INSPECTORSEQUENCECOMPONENT_H

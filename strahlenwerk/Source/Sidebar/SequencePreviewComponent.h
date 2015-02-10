#ifndef SEQUENCEPREVIEWCOMPONENT_H
#define SEQUENCEPREVIEWCOMPONENT_H

#include <juce>

class TimelineData;
class ZoomFactor;
class AudioManager;
class KeyframeComponent;

class SequencePreviewComponent :
	public Component
{
	public:
		SequencePreviewComponent();
		SequencePreviewComponent(ValueTree sequenceData_);

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

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencePreviewComponent)
};

#endif // SEQUENCEPREVIEWCOMPONENT_H

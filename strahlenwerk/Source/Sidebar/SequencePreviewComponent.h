#ifndef SEQUENCEPREVIEWCOMPONENT_H
#define SEQUENCEPREVIEWCOMPONENT_H

#include <juce>

class TimelineData;
class ZoomFactor;
class AudioManager;

class SequencePreviewComponent :
	public Component
{
	public:
		SequencePreviewComponent();
		SequencePreviewComponent(ValueTree sequenceData_);

		void paint(Graphics& g) override;
		void paintOverChildren(Graphics& g) override;

		void setSequenceData(ValueTree sequenceData_);

	private:
		ValueTree sequenceData;
		TimelineData& data;
		AudioManager& audioManager;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencePreviewComponent)
};

#endif // SEQUENCEPREVIEWCOMPONENT_H

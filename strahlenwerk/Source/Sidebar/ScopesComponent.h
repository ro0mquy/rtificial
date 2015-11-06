#ifndef SCOPESCOMPONENT_H
#define SCOPESCOMPONENT_H

#include <juce>

#include <Rendering/PostprocPipeline.h>

class HistogramComponent;

class ScopesComponent :
	public Component,
	private ChangeListener
{
	public:
		ScopesComponent();
		~ScopesComponent();

		void changeListenerCallback(ChangeBroadcaster* /*source*/) override;

	private:
		void resized() override;
		void visibilityChanged() override;

		ScopedPointer<ConcertinaPanel> concertinaPanel;
		ScopedPointer<HistogramComponent> histogramComponent;

		std::unique_ptr<PostprocPipeline> postprocPipeline;
		Image Image;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopesComponent)
};

class HistogramComponent :
	public Component
{
	public:
		HistogramComponent();

	private:
		void paint(Graphics& g) override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HistogramComponent)
};

#endif // SCOPESCOMPONENT_H

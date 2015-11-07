#ifndef SCOPESCOMPONENT_H
#define SCOPESCOMPONENT_H

#include <juce>

class Renderer;

class HistogramComponent;

class ScopesComponent :
	public Component,
	private ChangeListener,
	private AsyncUpdater
{
	public:
		ScopesComponent();
		~ScopesComponent();

		void changeListenerCallback(ChangeBroadcaster* /*source*/) override;

	private:
		void resized() override;
		void visibilityChanged() override;

		void handleAsyncUpdate() override;

		Renderer* renderer;

		ScopedPointer<ConcertinaPanel> concertinaPanel;
		ScopedPointer<HistogramComponent> histogramComponent;

		Image image;

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

#ifndef SCOPESCOMPONENT_H
#define SCOPESCOMPONENT_H

#include <juce>

class Renderer;
class TimelineData;
class AudioManager;

class HistogramComponent;

class ScopesComponent :
	public Component,
	private ChangeListener,
	private ValueTree::Listener,
	private AsyncUpdater
{
	public:
		ScopesComponent();
		~ScopesComponent();

		void changeListenerCallback(ChangeBroadcaster* /*source*/) override;
		void valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& /*property*/) override;
		void valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) override;
		void valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) override;
		void valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) override;
		void valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) override;

	private:
		void resized() override;
		void visibilityChanged() override;

		void handleAsyncUpdate() override;

		void addAsListener();
		void removeAsListener();

		Renderer* renderer;
		TimelineData& data;
		AudioManager& audioManager;

		ScopedPointer<ConcertinaPanel> concertinaPanel;
		ScopedPointer<HistogramComponent> histogramComponent;

		Image image;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopesComponent)
};

class HistogramComponent :
	public Component
{
	public:
		HistogramComponent(Image& image_);

	private:
		void paint(Graphics& g) override;

		Image& image;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HistogramComponent)
};

#endif // SCOPESCOMPONENT_H

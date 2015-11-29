#ifndef SCOPESCOMPONENT_H
#define SCOPESCOMPONENT_H

#include <juce>

class Renderer;
class TimelineData;
class AudioManager;

class HistogramComponent;
class VectorscopeComponent;
class WaveformComponent;

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
		ScopedPointer<VectorscopeComponent> vectorscopeComponent;
		ScopedPointer<WaveformComponent> waveformComponent;

		Image image;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopesComponent)
};

class HistogramComponent :
	public Component,
	private Value::Listener
{
	public:
		void resized() override;
		void paint(Graphics& g) override;

		HistogramComponent(Image& image_);

		enum ColourIds {
			backgroundColourId = 0x35fe2,
			lumaColourId,
			redColourId,
			greenColourId,
			blueColourId,
			rulerColourId,
		};

	private:
		void valueChanged(Value& /*value*/) override;

		Image& image;

		PropertyPanel propertyPanel;
		Value mode;
		Value logScale;

		int padding = 25;

		enum displayModes {
			LumaMode = 0x34ee2,
			RMode,
			GMode,
			BMode,
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HistogramComponent)
};

class VectorscopeComponent :
	public Component,
	private Value::Listener
{
	public:
		VectorscopeComponent(Image& image_);

		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds {
			backgroundColourId = 0x36fe2,
			graticuleColourId,
			pointColourId,
		};

	private:
		void valueChanged(Value& /*value*/) override;
		float calcAngle(float fraction);

		Image& image;

		PropertyPanel propertyPanel;
		Value mode;
		Value zoom;

		int padding = 25;

		enum displayModes {
			coloredMode,
			monochromeMode,
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VectorscopeComponent)
};

class WaveformComponent :
	public Component,
	private Value::Listener
{
	public:
		WaveformComponent(Image& image_);

		void resized() override;
		void paint(Graphics& g) override;

		enum ColourIds {
			backgroundColourId = 0x37fe2,
			lumaColourId,
			redColourId,
			greenColourId,
			blueColourId,
			rulerColourId,
		};

	private:
		void valueChanged(Value& /*value*/) override;

		Image& image;

		PropertyPanel propertyPanel;
		Value mode;

		int padding = 25;

		enum displayModes {
			lumaMode,
			rMode,
			gMode,
			bMode,
			RGBParadeMode,
		};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent)
};

#endif // SCOPESCOMPONENT_H

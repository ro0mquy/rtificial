#ifndef RT_COLOR_PICKER_H
#define RT_COLOR_PICKER_H

#include <juce>

#include <RtColor.h>

class RtColorSelector :
	public Component,
	public ChangeBroadcaster
{
	public:
		RtColorSelector();
		~RtColorSelector();

		RtColor getCurrentColor() const;
		void setCurrentColor(RtColor color, NotificationType notificationType = sendNotification);

		enum ColourIds {
			backgroundColourId = 0x1007000,
			textColourId = 0x1007001
		};

	private:
		class ColorSpaceView;
		class LumaSelectorComponent;
		class ColorSpaceMarker;
		class LumaSelectorMarker;
		friend class ColorSpaceView;
		friend struct ContainerDeletePolicy<ColorSpaceView>;
		friend class LumaSelectorComponent;
		friend struct ContainerDeletePolicy<LumaSelectorComponent>;

		ScopedPointer<ColorSpaceView> colorSpace;
		ScopedPointer<LumaSelectorComponent> lumaSelector;
		ScopedPointer<Label> helpText;

		float hue, chroma, luma;
		int edgeGap = 4;

		void setLuma(float y);
		void setHC(float h, float c);

		void update(NotificationType);
		void paint(Graphics&) override;
		void resized() override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RtColorSelector)
};

#endif // RT_COLOR_PICKER_H

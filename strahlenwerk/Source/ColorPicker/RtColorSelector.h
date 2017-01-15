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
		void setCurrentColor(RtColor newColor, NotificationType notificationType = sendNotification);

		enum ColourIds {
			backgroundColourId = 0x1007000
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

		RtColor color;
		ScopedPointer<ColorSpaceView> colorSpace;
		ScopedPointer<LumaSelectorComponent> lumaSelector;
		int edgeGap = 4;

		void setLuma(float newH);
		void setHC(float newH, float newC);

		void update(NotificationType);
		void paint(Graphics&) override;
		void resized() override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RtColorSelector)
};

#endif // RT_COLOR_PICKER_H

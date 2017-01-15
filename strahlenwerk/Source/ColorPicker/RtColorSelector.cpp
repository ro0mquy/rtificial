#include "RtColorSelector.h"

// TODO fix problems with full white
// TODO undo support
// TODO safe hcy channels seperately, don't reset on black
// TODO fix assertions in RtColor when picking from the wheel's edge
// TODO high-precision mode when holding shift

class RtColorSelector::ColorSpaceMarker :
	public Component
{
	public:
		ColorSpaceMarker() {
			setInterceptsMouseClicks(false, false);
		}

		void paint (Graphics& g) override {
			g.setColour(Colour::greyLevel(0.1f));
			g.drawEllipse(1.0f, 1.0f, getWidth() - 2.0f, getHeight() - 2.0f, 1.0f);
			g.setColour(Colour::greyLevel(0.9f));
			g.drawEllipse(2.0f, 2.0f, getWidth() - 4.0f, getHeight() - 4.0f, 1.0f);
		}

	private:
		JUCE_DECLARE_NON_COPYABLE(ColorSpaceMarker)
};

class RtColorSelector::ColorSpaceView :
	public Component
{
	public:
		ColorSpaceView(RtColorSelector& cs, const int edgeSize) :
			owner(cs),
			lastLuma(0.0f),
			edge(edgeSize)
		{
			addAndMakeVisible(marker);
		}

		void paint(Graphics& g) override {
			if (wheel.isNull()) {
				const float luma = owner.getCurrentColor().getLuma();
				const int dim = getWidth() * 2;
				const int radius = dim / 2;

				wheel = Image(Image::ARGB, dim, dim, true);
				Image::BitmapData pixels(wheel, Image::BitmapData::writeOnly);

				for (int y = -radius + 1; y <= radius; y++) {
					for (int x = -radius; x < radius; x++) {
						const float rho = sqrt(x*x + y*y);

						if (rho <= radius) {
							float phi = atan2(y, x);
							if (phi < 0) {
								phi = 2 * float_Pi - abs(phi);
							}

							const float hue = abs(phi) / (2.0f*float_Pi);
							const float chroma = rho / float(radius);

							if (x+radius >= dim || radius-y >= dim)
							std::cout << "dim\t" << dim << "\n"
								<< "x+r\t" << x + radius << "\n"
								<< "r-y\t" << radius - y << "\n";
							pixels.setPixelColour(
								x + radius,
								radius - y,
								Colour(RtColor::fromHCY(hue, chroma, luma))
							);
							if (x+radius >= dim || radius-y >= dim)
							std::cout << "\n";
						}
					}
				}
			}

			g.setOpacity(1.0f);
			g.drawImageTransformed(
				wheel,
				RectanglePlacement(RectanglePlacement::stretchToFit).getTransformToFit(
					wheel.getBounds().toFloat(),
					getLocalBounds().reduced(edge).toFloat()
				),
				false
			);
		}

		void mouseMove(const MouseEvent& e) override {
			const int radius = getWidth() / 2 - edge;
			const int x = e.x - radius - edge;
			const int y = radius + edge - e.y;

			if (x*x + y*y <= radius*radius) {
				setMouseCursor(MouseCursor::CrosshairCursor);
			} else {
				setMouseCursor(MouseCursor::NormalCursor);
			}
		}

		void mouseDown(const MouseEvent& e) override {
			mouseDrag(e);
		}

		void mouseDrag(const MouseEvent& e) override {
			const int radius = getWidth() / 2 - edge;

			const int downX = e.getMouseDownX() - radius - edge;
			const int downY = radius + edge - e.getMouseDownY();
			if (downX*downX + downY*downY <= radius*radius) {
				const int x = e.x - radius - edge;
				const int y = radius + edge - e.y;

				const float rho = sqrt(x*x + y*y);
				float phi = atan2(y, x);
				if (phi < 0) {
					phi = 2*float_Pi - abs(phi);
				}

				const float hue = abs(phi) / (2.0f*float_Pi);
				const float chroma = jmin(rho / float(radius), 1.0f);

				owner.setHC(hue, chroma);
			}
		}

		void updateIfNeeded() {
			float luma = owner.getCurrentColor().getLuma();

			if (lastLuma != luma) {
				lastLuma = luma;
				wheel = Image();
				repaint();
			}

			updateMarker();
		}

		void resized() override {
			wheel = Image();
			updateMarker();
		}

	private:
		RtColorSelector& owner;
		float lastLuma;
		ColorSpaceMarker marker;
		const int edge;
		Image wheel;

		void updateMarker() {
			const int radius = getWidth()/2 - edge;
			const RtColor color = owner.getCurrentColor();
			const float phi = color.getHue() * 2.0f*float_Pi;
			const float rho = color.getChroma() * radius;

			const int x = roundToInt(rho * cos(phi)) + radius;
			const int y = radius - roundToInt(rho * sin(phi));

			marker.setBounds(
				x,
				y,
				2*edge,
				2*edge
			);
		}

		JUCE_DECLARE_NON_COPYABLE(ColorSpaceView)
};

class RtColorSelector::LumaSelectorMarker :
	public Component
{
	public:
		LumaSelectorMarker(RtColorSelector& cs, float edge_) :
			colorSelector(cs),
			edge(edge_)
		{
			setInterceptsMouseClicks(false, false);
		}

		void paint (Graphics& g) override {
			const float cornerRadius = 4.0f;

			g.setColour(Colours::white.withAlpha(0.75f));
			g.fillRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);

			g.setColour(Colours::black.withAlpha(0.5f));
			g.drawVerticalLine(edge-2, edge, getHeight()-edge);
			g.drawVerticalLine(edge+2, edge, getHeight()-edge);

			g.setColour(Colour(colorSelector.getCurrentColor()));
			g.fillRect(int(edge)-1, int(edge), 3, getHeight()-int(2*edge));
		}

	private:
		RtColorSelector& colorSelector;
		float edge;

		JUCE_DECLARE_NON_COPYABLE(LumaSelectorMarker)
};

class RtColorSelector::LumaSelectorComponent :
	public Component
{
	public:
		LumaSelectorComponent(RtColorSelector& cs, const int edgeSize) :
			owner(cs),
			marker(cs, edgeSize),
			edge(edgeSize)
		{
			addAndMakeVisible(marker);
		}

		void paint(Graphics& g) override {
			ColourGradient grad;
			grad.isRadial = false;
			grad.point1.setXY(float(edge), 0.0f);
			grad.point2.setXY(float(getWidth() - edge), 0.0f);

			const RtColor currentColor = owner.getCurrentColor();
			const float hue = currentColor.getHue();
			const float chroma = currentColor.getChroma();

			for (float luma = 0.0f; luma <= 1.0f; luma += 0.02f) {
				grad.addColour(luma, Colour(RtColor::fromHCY(hue, chroma, luma)));
			}

			g.setGradientFill(grad);
			g.fillRect(getLocalBounds().reduced(edge));
		}

		void resized() override {
			const float luma = owner.getCurrentColor().getLuma();
			marker.setBounds(roundToInt((getWidth() - 2*edge - 1) * luma), 0, 2*edge + 1, getHeight());
		}

		void mouseDown(const MouseEvent& e) override {
			mouseDrag(e);
		}

		void mouseDrag(const MouseEvent& e) override {
			owner.setLuma((e.x - edge) / float(getWidth() - 2*edge));
		}

		void updateIfNeeded() {
			resized();
		}

	private:
		RtColorSelector& owner;
		LumaSelectorMarker marker;
		const int edge;

		JUCE_DECLARE_NON_COPYABLE(LumaSelectorComponent)
};


RtColorSelector::RtColorSelector() :
	color(Colours::white)
{
	addAndMakeVisible(colorSpace = new ColorSpaceView(*this, 7));
	addAndMakeVisible(lumaSelector = new LumaSelectorComponent(*this, 5));

	update(dontSendNotification);
}

RtColorSelector::~RtColorSelector() {
	dispatchPendingMessages();
}

RtColor RtColorSelector::getCurrentColor() const {
	return color;
}

void RtColorSelector::setCurrentColor(RtColor c, NotificationType notification) {
	if (c != color) {
		color = c;
		update(notification);
	}
}

void RtColorSelector::setLuma (float newL) {
	newL = jlimit(0.0f, 1.0f, newL);

	if (color.getLuma() != newL) {
		color = color.withLuma(newL);
		update(sendNotification);
	}
}

void RtColorSelector::setHC(float newH, float newC) {
	newH = jlimit(0.0f, 1.0f, newH);
	newC = jlimit(0.0f, 1.0f, newC);

	if (color.getHue() != newH || color.getChroma() != newC) {
		color = color.withHue(newH).withChroma(newC);
		lumaSelector->repaint();
		update(sendNotification);
	}
}

void RtColorSelector::update(NotificationType notification) {
	if (colorSpace != nullptr) {
		colorSpace->updateIfNeeded();
		lumaSelector->updateIfNeeded();
	}

	if (notification != dontSendNotification) {
		sendChangeMessage();
	}

	if (notification == sendNotificationSync) {
		dispatchPendingMessages();
	}
}

void RtColorSelector::paint(Graphics& g) {
	g.fillAll(findColour(backgroundColourId));
}

void RtColorSelector::resized() {
	const int lumaHeight = jmin(30, proportionOfHeight(0.15f));
	const int colorSpaceDim = jmin(getWidth(), getHeight() - edgeGap - lumaHeight) - 2*edgeGap;

	colorSpace->setBounds(
		edgeGap,
		edgeGap,
		colorSpaceDim,
		colorSpaceDim
	);

	lumaSelector->setBounds(
		edgeGap,
		colorSpace->getBottom() + edgeGap,
		getWidth() - 2*edgeGap,
		lumaHeight
	);
}


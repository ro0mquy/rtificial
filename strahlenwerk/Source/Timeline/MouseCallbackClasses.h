#ifndef MOUSECALLBACKCLASSES
#define MOUSECALLBACKCLASSES

#include <juce>

// some classes to pass mouse events to parent components
// if the current component don't want to receive the event

class McbComponent : public Component {
	public:
		void mouseEnter(const MouseEvent& e) override;
		void mouseExit(const MouseEvent& e) override;
		void mouseDown(const MouseEvent& e) override;
		void mouseUp(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void mouseMove(const MouseEvent& e) override;
		void mouseDoubleClick(const MouseEvent& e) override;
};

class McbLabel : public Label {
	public:
		void mouseEnter(const MouseEvent& e) override;
		void mouseExit(const MouseEvent& e) override;
		void mouseDown(const MouseEvent& e) override;
		void mouseUp(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void mouseMove(const MouseEvent& e) override;
		void mouseDoubleClick(const MouseEvent& e) override;
};

#endif // MOUSECALLBACKCLASSES

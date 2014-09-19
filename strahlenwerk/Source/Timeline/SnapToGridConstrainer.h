#ifndef SNAPTOGRIDCONSTRAINER_H
#define SNAPTOGRIDCONSTRAINER_H

#include <juce>

class ZoomFactor;

// all hail the tab completion
class SnapToGridConstrainer : public ComponentBoundsConstrainer {
	public:
		SnapToGridConstrainer();
		void checkBounds (Rectangle<int>& bounds,
				const Rectangle<int>& old,
				const Rectangle<int>& limits,
				const bool isStretchingTop,
				const bool isStretchingLeft,
				const bool isStretchingBottom,
				const bool isStretchingRight) override ;
		static float snapValueToGrid(const float value);

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SnapToGridConstrainer);
};

#endif // SNAPTOGRIDCONSTRAINER_H

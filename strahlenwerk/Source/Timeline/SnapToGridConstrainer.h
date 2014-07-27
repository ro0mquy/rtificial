#ifndef SNAPTOGRIDCONSTRAINER_H
#define SNAPTOGRIDCONSTRAINER_H

#include "../../JuceLibraryCode/JuceHeader.h"

// all hail the tab completion
class SnapToGridConstrainer : public ComponentBoundsConstrainer {
	public:
		SnapToGridConstrainer();
		void setGridWidth(const int width);
		void checkBounds (Rectangle<int>& bounds,
				const Rectangle<int>& old,
				const Rectangle<int>& limits,
				const bool isStretchingTop,
				const bool isStretchingLeft,
				const bool isStretchingBottom,
				const bool isStretchingRight) override ;

	private:
		int gridWidth;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SnapToGridConstrainer);
};

#endif // SNAPTOGRIDCONSTRAINER_H

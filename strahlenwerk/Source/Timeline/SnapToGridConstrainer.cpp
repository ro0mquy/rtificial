#include "SnapToGridConstrainer.h"

SnapToGridConstrainer::SnapToGridConstrainer() :
	gridWidth(1)
{
}

void SnapToGridConstrainer::setGridWidth(const int width) {
	gridWidth = width;
}

void SnapToGridConstrainer::checkBounds (Rectangle<int>& bounds,
		const Rectangle<int>& old,
		const Rectangle<int>& limits,
		const bool isStretchingTop,
		const bool isStretchingLeft,
		const bool isStretchingBottom,
		const bool isStretchingRight)
{
	// call original checkBounds() function
	ComponentBoundsConstrainer::checkBounds(bounds,
			old,
			limits,
			isStretchingTop,
			isStretchingLeft,
			isStretchingBottom,
			isStretchingRight);

	// snap to grid
	const float limit = float(bounds.getX()) / float(gridWidth);
	const int newX = roundFloatToInt(limit) * gridWidth;
	bounds.setX(newX);

	// don't change Y coordinate
	bounds.setY(old.getY());
}


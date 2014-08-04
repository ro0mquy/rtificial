#include "SnapToGridConstrainer.h"

SnapToGridConstrainer::SnapToGridConstrainer() :
	gridWidth(1)
{
}

void SnapToGridConstrainer::setGridWidth(const int width) {
	gridWidth = width;
}

int SnapToGridConstrainer::snapValueToGrid(int value) {
	const float posOnGrid = float(value) / float(gridWidth);
	const int newRoundedPos = roundFloatToInt(posOnGrid) * gridWidth;
	return newRoundedPos;
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
	if (isStretchingLeft) {
		const int newLeft = snapValueToGrid(bounds.getX());
		bounds.setLeft(newLeft);
	} else if (isStretchingRight) {
		const int newRight = snapValueToGrid(bounds.getRight());
		bounds.setRight(newRight);
	} else { // dragging
		const int newX = snapValueToGrid(bounds.getX());
		bounds.setX(newX);
	}

	// don't change Y coordinate
	bounds.setY(old.getY());
}


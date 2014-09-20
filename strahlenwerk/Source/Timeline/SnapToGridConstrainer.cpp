#include "SnapToGridConstrainer.h"
#include "ZoomFactor.h"

SnapToGridConstrainer::SnapToGridConstrainer() {
}

// snaps values that are in time units
float SnapToGridConstrainer::snapValueToGrid(const float value) {
	const float gridWidth = ZoomFactor::getZoomFactor().getGridWith();
	const float posOnGrid = value / gridWidth;
	const float newRoundedPos = roundFloatToInt(posOnGrid) * gridWidth;
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

	ZoomFactor& zoomFactor = ZoomFactor::getZoomFactor();

	// snap to grid
	if (isStretchingLeft) {
		const float rawNewLeft = snapValueToGrid(bounds.getX() / zoomFactor);
		const int newLeft = roundFloatToInt(rawNewLeft * zoomFactor);
		bounds.setLeft(newLeft);
	} else if (isStretchingRight) {
		const float rawNewRight = snapValueToGrid(bounds.getRight() / zoomFactor);
		const int newRight = roundFloatToInt(rawNewRight * zoomFactor);
		bounds.setRight(newRight);
	} else { // dragging
		const float rawNewX = snapValueToGrid(bounds.getX() / zoomFactor);
		const int newX = roundFloatToInt(rawNewX * zoomFactor);
		bounds.setX(newX);
	}

	// don't change Y coordinate
	bounds.setY(old.getY());
}


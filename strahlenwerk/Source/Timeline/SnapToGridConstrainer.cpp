#include "SnapToGridConstrainer.h"
#include "ZoomFactor.h"

SnapToGridConstrainer::SnapToGridConstrainer() :
	gridWidth(1.)
{
}

void SnapToGridConstrainer::setGridWidth(const int width) {
	//gridWidth = width;
}

// snaps values that are in time units
float SnapToGridConstrainer::snapValueToGrid(const float value) {
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
		const int newLeft = snapValueToGrid(bounds.getX() / zoomFactor);
		bounds.setLeft(newLeft * zoomFactor);
	} else if (isStretchingRight) {
		const int newRight = snapValueToGrid(bounds.getRight() / zoomFactor);
		bounds.setRight(newRight * zoomFactor);
	} else { // dragging
		const int newX = snapValueToGrid(bounds.getX() / zoomFactor);
		bounds.setX(newX * zoomFactor);
	}

	// don't change Y coordinate
	bounds.setY(old.getY());
}


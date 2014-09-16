#include "MouseCallbackClasses.h"

void McbLabel::mouseEnter(const MouseEvent& e) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseEnter(e.getEventRelativeTo(getParentComponent()));
	}
}

void McbLabel::mouseExit(const MouseEvent& e) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseExit(e.getEventRelativeTo(getParentComponent()));
	}
}

void McbLabel::mouseDown(const MouseEvent& e) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()));
	}
}

// action in here
void McbLabel::mouseUp(const MouseEvent& e) {
    if (isEditableOnSingleClick()
         && e.mouseWasClicked()
         && contains (e.getPosition())
         && ! e.mods.isPopupMenu())
    {
        showEditor();
    } else {
		if (getParentComponent() != nullptr) {
			getParentComponent()->mouseUp(e.getEventRelativeTo(getParentComponent()));
		}
	}
}

void McbLabel::mouseDrag(const MouseEvent& e) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseDrag(e.getEventRelativeTo(getParentComponent()));
	}
}

void McbLabel::mouseMove(const MouseEvent& e) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseMove(e.getEventRelativeTo(getParentComponent()));
	}
}

// action in here
void McbLabel::mouseDoubleClick(const MouseEvent& e) {
    if (isEditableOnDoubleClick() && ! e.mods.isPopupMenu()) {
        showEditor();
	} else {
		if (getParentComponent() != nullptr) {
			getParentComponent()->mouseDoubleClick(e.getEventRelativeTo(getParentComponent()));
		}
	}
}

void McbLabel::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseWheelMove(e.getEventRelativeTo(getParentComponent()), wheel);
	}
}

void McbLabel::mouseMagnify(const MouseEvent& e, float magnifyAmount) {
    if (getParentComponent() != nullptr) {
        getParentComponent()->mouseMagnify(e.getEventRelativeTo(getParentComponent()), magnifyAmount);
	}
}

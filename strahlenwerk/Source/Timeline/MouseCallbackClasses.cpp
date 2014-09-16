#include "MouseCallbackClasses.h"

void McbComponent::mouseEnter(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseEnter(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbComponent::mouseExit(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseExit(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbComponent::mouseDown(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseDown(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbComponent::mouseUp(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseUp(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbComponent::mouseDrag(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseDrag(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbComponent::mouseMove(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseMove(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbComponent::mouseDoubleClick(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseDoubleClick(e.getEventRelativeTo(parent));
			return;
		}
	}
}



void McbLabel::mouseEnter(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseEnter(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbLabel::mouseExit(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseExit(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbLabel::mouseDown(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseDown(e.getEventRelativeTo(parent));
			return;
		}
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
		for (Component* parent = getParentComponent();
				parent != nullptr;
				parent = parent->getParentComponent()) {

			bool allowsClicks;
			bool allowsClicksOnChildren;
			parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

			if (allowsClicks) {
				parent->mouseUp(e.getEventRelativeTo(parent));
				return;
			}
		}
	}
}

void McbLabel::mouseDrag(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseDrag(e.getEventRelativeTo(parent));
			return;
		}
	}
}

void McbLabel::mouseMove(const MouseEvent& e) {
	for (Component* parent = getParentComponent();
			parent != nullptr;
			parent = parent->getParentComponent()) {

		bool allowsClicks;
		bool allowsClicksOnChildren;
		parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

		if (allowsClicks) {
			parent->mouseMove(e.getEventRelativeTo(parent));
			return;
		}
	}
}

// action in here
void McbLabel::mouseDoubleClick(const MouseEvent& e) {
    if (isEditableOnDoubleClick() && ! e.mods.isPopupMenu()) {
        showEditor();
	} else {
		for (Component* parent = getParentComponent();
				parent != nullptr;
				parent = parent->getParentComponent()) {

			bool allowsClicks;
			bool allowsClicksOnChildren;
			parent->getInterceptsMouseClicks(allowsClicks, allowsClicksOnChildren);

			if (allowsClicks) {
				parent->mouseDoubleClick(e.getEventRelativeTo(parent));
				return;
			}
		}
	}
}

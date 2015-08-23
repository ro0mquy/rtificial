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
    //if (isEditableOnDoubleClick() && ! e.mods.isPopupMenu()) {
    if (isEditableOnDoubleClick() && e.mods.isLeftButtonDown() && e.mods.isCommandDown()) {
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



McbResizableBorder::McbResizableBorder(Component* const componentToResize, ComponentBoundsConstrainer* const constrainer_) :
	ResizableBorderComponent(componentToResize, constrainer_)
{
}

// action in here
void McbResizableBorder::mouseEnter(const MouseEvent& e) {
	ResizableBorderComponent::mouseEnter(e);

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

void McbResizableBorder::mouseExit(const MouseEvent& e) {
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

// action in here
void McbResizableBorder::mouseDown(const MouseEvent& e) {
	if (e.mods == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		ResizableBorderComponent::mouseDown(e);
		// let parent handle resize start
		//return;
	}

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
void McbResizableBorder::mouseUp(const MouseEvent& e) {
	if (e.mods == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		ResizableBorderComponent::mouseUp(e);
		// let parent handle resize start
		//return;
	}

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

// action in here
void McbResizableBorder::mouseDrag(const MouseEvent& e) {
	if (e.mods == ModifierKeys(ModifierKeys::leftButtonModifier | ModifierKeys::commandModifier)) {
		ResizableBorderComponent::mouseDrag(e);
		return;
	}

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

// action in here
void McbResizableBorder::mouseMove(const MouseEvent& e) {
	ResizableBorderComponent::mouseMove(e);

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

void McbResizableBorder::mouseDoubleClick(const MouseEvent& e) {
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

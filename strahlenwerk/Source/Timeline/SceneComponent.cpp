#include "SceneComponent.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "ZoomFactor.h"
#include <StrahlenwerkApplication.h>
#include <RtificialLookAndFeel.h>

SceneComponent::SceneComponent(ValueTree _sceneData, ZoomFactor& zoomFactor_) :
	sceneData(_sceneData),
	data(TimelineData::getTimelineData()),
	zoomFactor(zoomFactor_),
	resizableBorder(this, &constrainer)
{
	setPositioner(new Positioner(*this, sceneData, data, zoomFactor));

	// don't drag over the parent's edges
	constrainer.setMinimumOnscreenAmounts(0xffff, 0xffff, 0xffff, 0xffff);
	constrainer.setMinimumWidth(zoomFactor.getGridWidth() * zoomFactor);

	// add a border resizer that allows resizing only on the left and right
	resizableBorder.setBorderThickness(BorderSize<int>(0, 5, 0, 5));
	addAndMakeVisible(resizableBorder);

	sceneData.addListener(this);
	data.getSelection().addChangeListener(this);
	zoomFactor.addChangeListener(this);
}

SceneComponent::Positioner::Positioner(Component& component, ValueTree sceneData_, TimelineData& data_, ZoomFactor& zoomFactor_) :
	Component::Positioner(component),
	sceneData(sceneData_),
	data(data_),
	zoomFactor(zoomFactor_)
{
}

SceneComponent::~SceneComponent() {
	sceneData.removeListener(this);
	data.getSelection().removeChangeListener(this);
	zoomFactor.removeChangeListener(this);
}

void SceneComponent::updateBounds() {
	const int start = (int) data.getSceneStart(sceneData) * zoomFactor;
	const int duration = (int) data.getSceneDuration(sceneData) * zoomFactor;

	const int padding = 0;
	setBounds(start, padding, duration, getParentHeight() - 2*padding);
}

void SceneComponent::Positioner::applyNewBounds(const Rectangle<int>& newBounds) {
	const bool xChanged = newBounds.getX() != getComponent().getX();
	const bool widthChanged = newBounds.getWidth() != getComponent().getWidth();

	if (xChanged && !widthChanged) {
		// dragging
		const int newX = newBounds.getX() / zoomFactor;
		const int newStart = zoomFactor.snapValueToGrid(newX);
		data.setSceneStart(sceneData, newStart);
	} else if (xChanged && widthChanged) {
		// stretching left
		const int newX = newBounds.getX() / zoomFactor;
		const int newStart = zoomFactor.snapValueToGrid(newX);

		// calculate the new width through the change of X
		// so that the right end stays where it is
		// otherwise the duration and not the right side would be snapped to grid
		const int oldStart = data.getSceneStart(sceneData);
		const int deltaStart = oldStart - newStart;
		const int oldDuration = data.getSceneDuration(sceneData);
		const int newDuration = oldDuration + deltaStart;

		data.setSceneStart(sceneData, newStart);
		data.setSceneDuration(sceneData, newDuration);
	} else if (!xChanged && widthChanged) {
		// stretching right
		const int newWidth = newBounds.getWidth() / zoomFactor;
		const int start = data.getSceneStart(sceneData);
		const int newAbsoluteEnd = start + newWidth;
		const int newAbsoluteEndGrid = zoomFactor.snapValueToGrid(newAbsoluteEnd);
		const int newDuration = newAbsoluteEndGrid - start;
		data.setSceneDuration(sceneData, newDuration);
	}
}

void SceneComponent::paint(Graphics& g) {
	RtificialLookAndFeel* laf = dynamic_cast<RtificialLookAndFeel*>(&getLookAndFeel());

	Rectangle<float> sceneRect = getLocalBounds().toFloat();
	sceneRect.removeFromBottom(1.0f);

	const String shaderSource = data.getSceneShaderSource(sceneData);
	const bool selected = data.getSelection().contains(sceneData);

	if (nullptr == laf) {
		Colour fillColor = findColour(SceneComponent::fillColourId);
		if (selected) {
			fillColor = findColour(SceneComponent::highlightedFillColourId);
		}
		g.fillAll(fillColor);

		g.setColour(findColour(SceneComponent::outlineColourId));
		g.drawRect(sceneRect, 1);

		g.setColour(findColour(SceneComponent::textColourId));
		g.drawFittedText(shaderSource, sceneRect.getSmallestIntegerContainer(), Justification::centred, 1);
	} else {
		laf->drawScene(g, sceneRect, selected, shaderSource);
	}
}

void SceneComponent::mouseDown(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.originalComponent == &resizableBorder && m.isLeftButtonDown() && m.isCommandDown()) {
		data.getUndoManager().beginNewTransaction("Resize Scene");
		return;
	}

	if (m.isLeftButtonDown() && m.isCommandDown()) {
		data.getUndoManager().beginNewTransaction("Drag Scene");
		beginDragAutoRepeat(10); // time between drag events
		startDraggingComponent(this, event);
	} else {
		McbComponent::mouseDown(event);
	}
}

void SceneComponent::mouseDrag(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (!event.mouseWasClicked() && m.isLeftButtonDown() && m.isCommandDown()) {
		dragComponent(this, event, &constrainer);

		// scroll viewport if necessary
		Viewport* parentViewport = findParentComponentOfClass<Viewport>();
		const MouseEvent viewportEvent = event.getEventRelativeTo(parentViewport);
		// scroll only X- not Y-Direction
		// current X position gets normally set
		// current Y position is a constant that is greater than the minimum distance to the border (21 > 20)
		parentViewport->autoScroll(viewportEvent.x, 21, 20, 5);
	} else {
		McbComponent::mouseDrag(event);
	}
}

void SceneComponent::mouseUp(const MouseEvent& event) {
	const ModifierKeys& m = event.mods;
	if (event.originalComponent == &resizableBorder && m.isLeftButtonDown() && m.isCommandDown()) {
		return;
	}

	if (event.mouseWasClicked() && m.isMiddleButtonDown() && m.isCommandDown()) {
		// delete scene
		data.getSelection().remove(sceneData);
		data.getUndoManager().beginNewTransaction("Delete Scene");
		data.removeScene(sceneData);
		// this component gets deleted after this, so don't do stupid things

	} else if (event.mouseWasClicked() && m.isCommandDown() && m.isPopupMenu()) {
		// select shader source
		const String currentShaderSource = data.getSceneShaderSource(sceneData);
		int currentShaderSourceId = 0;
		const std::vector<File> allSceneShaderFiles = StrahlenwerkApplication::getInstance()->getProject().getLoader().listSceneFiles();

		PopupMenu menu;
		const size_t numSceneShaderFiles = allSceneShaderFiles.size();
		for (size_t i = 0; i < numSceneShaderFiles; i++) {
			const String shaderName = allSceneShaderFiles[i].getFileNameWithoutExtension();
			const bool isTicked = currentShaderSource == shaderName;
			if (isTicked) { currentShaderSourceId = i+1; }
			menu.addItem(i+1, shaderName, true, isTicked);
		}

		const int menuResult = menu.showAt(this, currentShaderSourceId, getWidth(), 1, getHeight());
		if (menuResult == 0) {
			// user dismissed menu
			return;
		}

		// set shader source to selected one
		data.getUndoManager().beginNewTransaction("Change Shader Source");
		data.setSceneShaderSource(sceneData, allSceneShaderFiles[menuResult - 1].getFileNameWithoutExtension());

	} else if (event.mouseWasClicked() && m.isRightButtonDown() && !m.isAnyModifierKeyDown()) {
		// add sequence to selection
		data.getSelection().set(sceneData);

	} else {
		McbComponent::mouseUp(event);
	}
}

void SceneComponent::resized() {
	Rectangle<int> theBounds = getLocalBounds();
	theBounds.removeFromBottom(1);
	resizableBorder.setBounds(theBounds);
}

void SceneComponent::parentHierarchyChanged() {
	updateBounds();
}

void SceneComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &zoomFactor) {
		// zoomFactor update
		constrainer.setMinimumWidth(zoomFactor.getGridWidth() * zoomFactor);
		updateBounds();
	} else if (source == &data.getSelection()) {
		// selection update
		repaint();
	}
}

// ValueTree::Listener callbacks
void SceneComponent::valueTreePropertyChanged(ValueTree& /*parentTree*/, const Identifier& property) {
	// callback comes only from the sceneData that belongs to this component
	if (property == treeId::sceneStart || property == treeId::sceneDuration) {
		updateBounds();
	} else if (property == treeId::sceneShaderSource) {
		repaint();
	}
}

void SceneComponent::valueTreeChildAdded(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenAdded*/) {
}

void SceneComponent::valueTreeChildRemoved(ValueTree& /*parentTree*/, ValueTree& /*childWhichHasBeenRemoved*/, int /*indexFromWhichChildWasRemoved*/) {
}

void SceneComponent::valueTreeChildOrderChanged(ValueTree& /*parentTree*/, int /*oldIndex*/, int /*newIndex*/) {
}

void SceneComponent::valueTreeParentChanged(ValueTree& /*treeWhoseParentHasChanged*/) {
}

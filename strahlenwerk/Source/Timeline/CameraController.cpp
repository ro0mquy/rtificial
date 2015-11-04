#include "CameraController.h"

#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <AudioManager.h>
#include "TimelineData.h"
#include "Interpolator.h"
#include "TreeIdentifiers.h"

CameraController* CameraController::globalCameraController = nullptr;

CameraController::CameraController(TimelineData& data_, Interpolator& interpolator_) :
	SpecialUniformController(data_),
	interpolator(interpolator_),
	cameraFocalLengthName("camera_focal_length"),
	cameraPositionName("camera_position"),
	cameraRotationName("camera_rotation")
{
	globalCameraController = this;

	// register command listener
	MainWindow::getApplicationCommandManager().addListener(this);

	// add this as a key listener to the main window, as soon as it is constructed
	triggerAsyncUpdate();
}

void CameraController::handleAsyncUpdate() {
	MainWindow& mainWindow = StrahlenwerkApplication::getInstance()->getMainWindow();
	mainWindow.addKeyListener(this);
	mainWindow.getMainContentComponent().getOpenGLComponent().addMouseListener(this, true);
}

CameraController::~CameraController() {
	//MainWindow::getApplicationCommandManager().removeListener(this);
	// this segfaults because MainWindow gets deleted before this class
	//StrahlenwerkApplication::getInstance()->getMainWindow().removeKeyListener(this);
	globalCameraController = nullptr;
}

bool CameraController::wantControlUniform(String& uniformName) {
	std::lock_guard<std::mutex> lock(cameraMutex);
	if (hasControl) {
		return uniformName == cameraFocalLengthName ||
			uniformName == cameraPositionName ||
			uniformName == cameraRotationName;
	}
	return false;
}

Interpolator::UniformState CameraController::getUniformState(String& uniformName) {
	ValueTree tree(treeId::controlledValue);
	if (uniformName == cameraFocalLengthName) {
		cameraMutex.lock();
		const float tmpFocalLength = focalLength;
		cameraMutex.unlock();
		data.setFloatToValue(tree, tmpFocalLength, false);
	} else if (uniformName == cameraPositionName) {
		cameraMutex.lock();
		const glm::vec3 tmpPosition = position;
		cameraMutex.unlock();
		data.setVec3ToValue(tree, tmpPosition, false);
	} else if (uniformName == cameraRotationName) {
		cameraMutex.lock();
		const glm::quat tmpRotation = rotation;
		cameraMutex.unlock();
		data.setQuatToValue(tree, tmpRotation, false);
	}
	return Interpolator::UniformState(tree, false);
}

bool CameraController::keyPressed(const KeyPress& /*key*/, Component* /*originatingComponent*/) {
	return false;
}

bool CameraController::keyStateChanged(bool /*isKeyPressed*/, Component* /*originatingComponent*/) {
	const bool isCameraKeyDown =
		KeyPress::isKeyCurrentlyDown('w') ||
		KeyPress::isKeyCurrentlyDown('a') ||
		KeyPress::isKeyCurrentlyDown('s') ||
		KeyPress::isKeyCurrentlyDown('d') ||
		KeyPress::isKeyCurrentlyDown('e') ||
		KeyPress::isKeyCurrentlyDown('c') ||

		KeyPress::isKeyCurrentlyDown('i') ||
		KeyPress::isKeyCurrentlyDown('j') ||
		KeyPress::isKeyCurrentlyDown('k') ||
		KeyPress::isKeyCurrentlyDown('l') ||
		KeyPress::isKeyCurrentlyDown('u') ||
		KeyPress::isKeyCurrentlyDown('o');

	if (!isTimerRunning() && isCameraKeyDown) {
		// some keys are down now
		startTimerCallback();
	} else if (isTimerRunning() && !isCameraKeyDown) {
		// no camera key is pressed anymore
		stopTimerCallback();
	}

	const bool isShiftDown = ModifierKeys::getCurrentModifiers().isAltDown(); // if you change the modifier, also have a look into timerCallback()
	if (!shiftDraggingActive && isShiftDown) {
		// shift key is down
		shiftDraggingActive = true;
		Desktop::getInstance().addGlobalMouseListener(this);
		startMouseDragging();
	} else if (shiftDraggingActive && !isShiftDown) {
		// shift got released
		shiftDraggingActive = false;
		Desktop::getInstance().removeGlobalMouseListener(this);
		stopMouseDragging();
	}

	return false;
}

void CameraController::mouseMove(const MouseEvent& /*event*/) {
	// only when shift is held down, this class is registered as a global mouse listener
	// otherwise the events comes from the openGLComponent
	if (shiftDraggingActive) {
		handleMouseDragging();
	}
}

void CameraController::mouseDown(const MouseEvent& event) {
	// click into openGl component
	if (event.mods == ModifierKeys(ModifierKeys::leftButtonModifier)) {
		// start to control the camera with the mouse
		//Desktop::getInstance().getMainMouseSource().enableUnboundedMouseMovement(true); // alot of lag with this
		startMouseDragging();
	}
}

void CameraController::mouseDrag(const MouseEvent& event) {
	if (event.mods.isLeftButtonDown() || shiftDraggingActive) {
		handleMouseDragging();
	}
}

void CameraController::mouseUp(const MouseEvent& event) {
	if (event.mods.isLeftButtonDown()) {
		stopMouseDragging();
		//Desktop::getInstance().getMainMouseSource().enableUnboundedMouseMovement(false);
	}
}

void CameraController::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	// called when inside of openGLComponent or shift is down
	if (event.mods.isCommandDown()) {
		// zoom lens
		std::lock_guard<std::mutex> lock(cameraMutex);
		focalLength = cameraMath.focalLengthTuning(focalLength, wheel.deltaY * (wheel.isReversed ? -1 : 1));
		sendChangeMessage();
	} else {
		// change speed
		cameraMath.movementSpeedTuning(wheel.deltaY * (wheel.isReversed ? -1 : 1));
	}
}

void CameraController::timerCallback() {
	std::lock_guard<std::mutex> lock(cameraMutex);

	const double oldLastCallbackTime = lastCallbackTime;
	lastCallbackTime = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
	const float deltaTime = lastCallbackTime - oldLastCallbackTime;

	const ModifierKeys modKeys = ModifierKeys::getCurrentModifiers().withoutFlags(ModifierKeys::altModifier);
	if (modKeys.isAnyModifierKeyDown()) {
		// do nothing when any modifiers besides shift are held down
		return;
	}

	if (KeyPress::isKeyCurrentlyDown('w')) {
		position = cameraMath.positionForward(position, rotation, deltaTime);
	}
	if (KeyPress::isKeyCurrentlyDown('s')) {
		position = cameraMath.positionBackward(position, rotation, deltaTime);
	}

	if (KeyPress::isKeyCurrentlyDown('a')) {
		position = cameraMath.positionLeft(position, rotation, deltaTime);
	}
	if (KeyPress::isKeyCurrentlyDown('d')) {
		position = cameraMath.positionRight(position, rotation, deltaTime);
	}

	if (KeyPress::isKeyCurrentlyDown('e')) {
		position = cameraMath.positionUp(position, rotation, deltaTime);
	}
	if (KeyPress::isKeyCurrentlyDown('c')) {
		position = cameraMath.positionDown(position, rotation, deltaTime);
	}

	if (KeyPress::isKeyCurrentlyDown('i')) {
		rotation = cameraMath.rotationUp(position, rotation, deltaTime);
	}
	if (KeyPress::isKeyCurrentlyDown('k')) {
		rotation = cameraMath.rotationDown(position, rotation, deltaTime);
	}

	if (KeyPress::isKeyCurrentlyDown('j')) {
		rotation = cameraMath.rotationLeft(position, rotation, deltaTime);
	}
	if (KeyPress::isKeyCurrentlyDown('l')) {
		rotation = cameraMath.rotationRight(position, rotation, deltaTime);
	}

	if (KeyPress::isKeyCurrentlyDown('u')) {
		rotation = cameraMath.rotationCounterclockwise(position, rotation, deltaTime);
	}
	if (KeyPress::isKeyCurrentlyDown('o')) {
		rotation = cameraMath.rotationClockwise(position, rotation, deltaTime);
	}

	sendChangeMessage();
}

void CameraController::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case CameraController::setKeyframe:
			setKeyframeAtCurrentPosition();
			break;
		case CameraController::playPauseWithAnimation:
			releaseControl();
			break;
		case CameraController::playPauseWithoutAnimation:
			takeOverControl();
			break;
		case CameraController::resetCameraPosition:
			takeOverControl();
			cameraMutex.lock();
			position = glm::vec3(0., 0., 0.);
			cameraMutex.unlock();
			sendChangeMessage();
			break;
		case CameraController::resetCameraRotation:
			takeOverControl();
			cameraMutex.lock();
			rotation = glm::quat(1., 0., 0., 0.);
			cameraMutex.unlock();
			sendChangeMessage();
			break;
		case CameraController::increaseMovementSpeed:
			cameraMath.movementSpeedTuning(1);
			break;
		case CameraController::decreaseMovementSpeed:
			cameraMath.movementSpeedTuning(-1);
			break;
		case CameraController::increaseRotationSpeed:
			cameraMath.rotationSpeedTuning(1);
			break;
		case CameraController::decreaseRotationSpeed:
			cameraMath.rotationSpeedTuning(-1);
			break;
	}
}

void CameraController::applicationCommandListChanged() {
}

void CameraController::setKeyframeAtCurrentPosition() {
	if (!getHasControl()) {
		getCameraFromCurrentPosition();
	}

	data.getUndoManager().beginNewTransaction("Set Camera");

	const int currentTime = AudioManager::getAudioManager().getTime();

	ValueTree focalLengthUniform = data.getUniform(var(cameraFocalLengthName));
	ValueTree currentFocSequence = data.getSequenceForTime(focalLengthUniform, currentTime);
	if (currentFocSequence.isValid()) {
		const var relativeCurrentTime = currentTime - data.getAbsoluteStartForSequence(currentFocSequence);
		ValueTree keyframe = data.getKeyframe(currentFocSequence, relativeCurrentTime);
		if (!keyframe.isValid()) {
			keyframe = data.addKeyframe(currentFocSequence, relativeCurrentTime);
		}
		ValueTree keyframeValue = data.getKeyframeValue(keyframe);

		cameraMutex.lock();
		const float tmpFocalLength = focalLength;
		cameraMutex.unlock();

		data.setFloatToValue(keyframeValue, tmpFocalLength);
	}

	ValueTree positionUniform = data.getUniform(var(cameraPositionName));
	ValueTree currentPosSequence = data.getSequenceForTime(positionUniform, currentTime);
	if (currentPosSequence.isValid()) {
		const var relativeCurrentTime = currentTime - data.getAbsoluteStartForSequence(currentPosSequence);
		ValueTree keyframe = data.getKeyframe(currentPosSequence, relativeCurrentTime);
		if (!keyframe.isValid()) {
			keyframe = data.addKeyframe(currentPosSequence, relativeCurrentTime);
		}
		ValueTree keyframeValue = data.getKeyframeValue(keyframe);

		cameraMutex.lock();
		const glm::vec3 tmpPosition = position;
		cameraMutex.unlock();

		data.setVec3ToValue(keyframeValue, tmpPosition);
	}

	ValueTree rotationUniform = data.getUniform(var(cameraRotationName));
	ValueTree currentRotSequence = data.getSequenceForTime(rotationUniform, currentTime);
	if (currentRotSequence.isValid()) {
		const var relativeCurrentTime = currentTime - data.getAbsoluteStartForSequence(currentRotSequence);
		ValueTree keyframe = data.getKeyframe(currentRotSequence, relativeCurrentTime);
		if (!keyframe.isValid()) {
			keyframe = data.addKeyframe(currentRotSequence, relativeCurrentTime);
		}
		ValueTree keyframeValue = data.getKeyframeValue(keyframe);

		cameraMutex.lock();
		const glm::quat tmpRotation = rotation;
		cameraMutex.unlock();

		data.setQuatToValue(keyframeValue, tmpRotation);
	}

	releaseControl();
}

void CameraController::getCameraFromCurrentPosition() {
	ValueTree focalLengthUniform = data.getUniform(var(cameraFocalLengthName));
	ValueTree focalLengthValue = interpolator.getUniformStateFromTimelineData(focalLengthUniform).first;

	ValueTree positionUniform = data.getUniform(var(cameraPositionName));
	ValueTree positionValue = interpolator.getUniformStateFromTimelineData(positionUniform).first;

	ValueTree rotationUniform = data.getUniform(var(cameraRotationName));
	ValueTree rotationValue = interpolator.getUniformStateFromTimelineData(rotationUniform).first;

	const float tmpFocalLength = data.getFloatFromValue(focalLengthValue);
	const glm::vec3 tmpPosition = data.getVec3FromValue(positionValue);
	const glm::quat tmpRotation = data.getQuatFromValue(rotationValue);

	cameraMutex.lock();
	focalLength = tmpFocalLength;
	position = tmpPosition;
	rotation = tmpRotation;
	cameraMutex.unlock();
}

bool CameraController::getHasControl() {
	std::lock_guard<std::mutex> lock(cameraMutex);
	return hasControl;
}

void CameraController::setHasControl(const bool shouldHaveControl) {
	std::lock_guard<std::mutex> lock(cameraMutex);
	hasControl = shouldHaveControl;
}

void CameraController::takeOverControl() {
	if (!getHasControl()) {
		getCameraFromCurrentPosition();
		setHasControl(true);
		sendChangeMessage();
	}
}

void CameraController::releaseControl() {
	if (getHasControl()) {
		setHasControl(false);
		sendChangeMessage();
	}
}

void CameraController::startTimerCallback() {
	takeOverControl();
	startTimer(timerInterval);
	lastCallbackTime = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
}

void CameraController::stopTimerCallback() {
	stopTimer();
}

void CameraController::startMouseDragging() {
	takeOverControl();
	originalMousePos = Desktop::getMousePosition();
	lastMouseMoveTime = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
}

void CameraController::stopMouseDragging() {
	Desktop::setMousePosition(originalMousePos);
}

void CameraController::handleMouseDragging() {
	const Point<int> currentMousePos = Desktop::getMousePosition();
	const Point<int> deltaMousePos = currentMousePos - originalMousePos;
	if (deltaMousePos.isOrigin()) {
		return;
	}

	// reset position, so the mouse doesn't move
	Desktop::setMousePosition(originalMousePos);

	const double oldLastMouseMoveTime = lastMouseMoveTime;
	lastMouseMoveTime = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
	const float deltaTime = lastMouseMoveTime - oldLastMouseMoveTime;

	cameraMutex.lock();
	rotation = cameraMath.mouseMove(position, rotation, deltaTime, glm::vec2(deltaMousePos.x, -deltaMousePos.y)); // origin in bottom left corner
	cameraMutex.unlock();

	sendChangeMessage();
}

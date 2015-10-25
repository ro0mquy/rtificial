#include "CameraController.h"

#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <AudioManager.h>
#include "TimelineData.h"
#include "Interpolator.h"
#include "TreeIdentifiers.h"

CameraController* CameraController::globalCameraController = nullptr;

CameraController::CameraController(TimelineData& data_, Interpolator& interpolator_) :
	SpecialUniformController(data_),
	interpolator(interpolator_),
	hasControl(false),
	cameraPositionName("camera_position"),
	cameraRotationName("camera_rotation")
{
	globalCameraController = this;

	// register command listener
	MainWindow::getApplicationCommandManager().addListener(this);

	// add this as a key listener to the main window, as soon as it is constructed
	triggerAsyncUpdate();
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
		return uniformName == cameraPositionName ||
			uniformName == cameraRotationName;
	}
	return false;
}

Interpolator::UniformState CameraController::getUniformState(String& uniformName) {
	ValueTree tree(treeId::controlledValue);
	if (uniformName == cameraPositionName) {
		cameraMutex.lock();
		glm::vec3 tmpPosition = position;
		cameraMutex.unlock();
		data.setVec3ToValue(tree, tmpPosition, false);
	} else if (uniformName == cameraRotationName) {
		cameraMutex.lock();
		glm::quat tmpRotation = rotation;
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
		takeOverControl();
		std::lock_guard<std::mutex> lock(cameraMutex);
		startTimer(timerInterval);
		lastCallback = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
	} else if (!isCameraKeyDown) {
		// no camera key is pressed anymore
		stopTimer();
	}

	return false;
}

void CameraController::handleAsyncUpdate() {
	StrahlenwerkApplication::getInstance()->getMainWindow().addKeyListener(this);
}

void CameraController::timerCallback() {
	std::lock_guard<std::mutex> lock(cameraMutex);

	const double oldLastCallback = lastCallback;
	lastCallback = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
	const float deltaTime = lastCallback - oldLastCallback;

	const ModifierKeys modKeys = ModifierKeys::getCurrentModifiers();
	if (modKeys.isAnyModifierKeyDown()) {
		// do nothing when any modifiers are held down
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
		glm::vec3 tmpPosition = position;
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
		glm::quat tmpRotation = rotation;
		cameraMutex.unlock();

		data.setQuatToValue(keyframeValue, tmpRotation);
	}
}

void CameraController::getCameraFromCurrentPosition() {
	ValueTree positionUniform = data.getUniform(var(cameraPositionName));
	ValueTree positionValue = interpolator.getUniformStateFromTimelineData(positionUniform).first;

	ValueTree rotationUniform = data.getUniform(var(cameraRotationName));
	ValueTree rotationValue = interpolator.getUniformStateFromTimelineData(rotationUniform).first;

	glm::vec3 tmpPosition = data.getVec3FromValue(positionValue);
	glm::quat tmpRotation = data.getQuatFromValue(rotationValue);

	cameraMutex.lock();
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

#include "SpecialUniformController.h"
#include <AudioManager.h>
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include <StrahlenwerkApplication.h>
#include <MainWindow.h>

bool TimeController::shouldAddUniformToTimlineData(String& uniformName) {
	return uniformName != "time";
}

bool TimeController::wantControlUniform(String& uniformName) {
	return uniformName == "time";
}

Interpolator::UniformState TimeController::getUniformState(String& /*uniformName*/) {
	const float currentTime = AudioManager::getAudioManager().getTimeInBeats();
	ValueTree timeValue(treeId::controlledValue);
	data.setFloatToValue(timeValue, currentTime);
	return Interpolator::UniformState(timeValue, false);
}


CameraController::CameraController(TimelineData& data_) :
	SpecialUniformController(data_)
{
	// add this as a key listener to the main window, as soon as it is constructed
	triggerAsyncUpdate();
}

CameraController::~CameraController() {
	StrahlenwerkApplication::getInstance()->getMainWindow().removeKeyListener(this);
}

bool CameraController::wantControlUniform(String& uniformName) {
	return uniformName == "camera_position" ||
		uniformName == "camera_rotation";
}

Interpolator::UniformState CameraController::getUniformState(String& uniformName) {
	ValueTree tree(treeId::controlledValue);
	if (uniformName == "camera_position") {
		data.setVec3ToValue(tree, position);
	} else if (uniformName == "camera_rotation") {
		data.setQuatToValue(tree, rotation);
	}
	return Interpolator::UniformState(tree, false);
}

bool CameraController::keyPressed(const KeyPress& key, Component* /*originatingComponent*/) {
	bool isCameraKey =
		key.isKeyCode('w') ||
		key.isKeyCode('a') ||
		key.isKeyCode('s') ||
		key.isKeyCode('d') ||
		key.isKeyCode('e') ||
		key.isKeyCode('c') ||

		key.isKeyCode('i') ||
		key.isKeyCode('j') ||
		key.isKeyCode('k') ||
		key.isKeyCode('l') ||
		key.isKeyCode('u') ||
		key.isKeyCode('o');

	if (isCameraKey && !isTimerRunning()) {
		std::lock_guard<std::mutex> lock(cameraMutex);
		startTimer(timerInterval);
		lastCallback = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
	}

	return isCameraKey;
}

bool CameraController::keyStateChanged(bool isKeyPressed, Component* /*originatingComponent*/) {
	if (isKeyPressed || !isTimerRunning()) {
		// key gets pressed or no camera key was down
		return false;
	}

	bool isCameraKeyDown =
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

	if (!isCameraKeyDown) {
		// no camera key is pressed anymore
		stopTimer();
	}

	return !isCameraKeyDown;
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
}

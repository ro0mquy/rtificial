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

#include <glm/glm.hpp>
Interpolator::UniformState CameraController::getUniformState(String& uniformName) {
	ValueTree tree(treeId::controlledValue);
	if (uniformName == "camera_position") {
		data.setVec3ToValue(tree, glm::vec3(0., 0., 0.));
	} else if (uniformName == "camera_rotation") {
		data.setVec4ToValue(tree, glm::vec4(0., 0., 0., 1.));
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
		startTimer(timerInterval);
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
	puts("\\o/");
}

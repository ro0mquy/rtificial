#include "CameraController.h"

#include <StrahlenwerkApplication.h>
#include <MainWindow.h>
#include <MainContentComponent.h>
#include <AudioManager.h>
#include "TimelineData.h"
#include "Interpolator.h"
#include "TreeIdentifiers.h"

#include <glm/gtx/quaternion.hpp>

CameraController* CameraController::globalCameraController = nullptr;

CameraController::CameraController(TimelineData& data_, Interpolator& interpolator_) :
	SpecialUniformController(data_),
	interpolator(interpolator_),
	cameraPositionName("camera_position"),
	cameraRotationName("camera_rotation"),
	cameraFocalLengthName("camera_focal_length"),
	spectatormodeActiveName("spectatormode_active"),
	spectatormodePositionName("spectatormode_position"),
	spectatormodeRotationName("spectatormode_rotation"),
	spectatormodeFocalLengthName("spectatormode_focal_length")
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

bool CameraController::shouldAddUniformToTimlineData(String& uniformName) {
	return ! (uniformName == spectatormodeActiveName ||
			uniformName == spectatormodePositionName ||
			uniformName == spectatormodeRotationName ||
			uniformName == spectatormodeFocalLengthName);
}

bool CameraController::wantControlUniform(String& uniformName) {
	return (uniformName == spectatormodeActiveName ||
			uniformName == spectatormodePositionName ||
			uniformName == spectatormodeRotationName ||
			uniformName == spectatormodeFocalLengthName);
}

Interpolator::UniformState CameraController::getUniformState(String& uniformName) {
	ValueTree tree(treeId::controlledValue);
	if (uniformName == spectatormodeActiveName) {
		data.setBoolToValue(tree, getHasControl(), false);
	} else if (uniformName == spectatormodePositionName) {
		cameraMutex.lock();
		const glm::vec3 tmpPosition = position;
		cameraMutex.unlock();
		data.setVec3ToValue(tree, tmpPosition, false);
	} else if (uniformName == spectatormodeRotationName) {
		cameraMutex.lock();
		const glm::quat tmpRotation = rotation;
		cameraMutex.unlock();
		data.setQuatToValue(tree, tmpRotation, false);
	} else if (uniformName == spectatormodeFocalLengthName) {
		cameraMutex.lock();
		const float tmpFocalLength = focalLength;
		cameraMutex.unlock();
		data.setFloatToValue(tree, tmpFocalLength, false);
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

	//* reenable this comment for disabling of [alt] mouse dragging
	// more code for shift mouse control in mouseMove(), mouseDrag()
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
	// */

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
		position = cameraMath.positionForward(position, rotation, deltaTime, focalLength);
	}
	if (KeyPress::isKeyCurrentlyDown('s')) {
		position = cameraMath.positionBackward(position, rotation, deltaTime, focalLength);
	}

	if (KeyPress::isKeyCurrentlyDown('a')) {
		position = cameraMath.positionLeft(position, rotation, deltaTime, focalLength);
	}
	if (KeyPress::isKeyCurrentlyDown('d')) {
		position = cameraMath.positionRight(position, rotation, deltaTime, focalLength);
	}

	if (KeyPress::isKeyCurrentlyDown('e')) {
		position = cameraMath.positionUp(position, rotation, deltaTime, focalLength);
	}
	if (KeyPress::isKeyCurrentlyDown('c')) {
		position = cameraMath.positionDown(position, rotation, deltaTime, focalLength);
	}

	if (KeyPress::isKeyCurrentlyDown('i')) {
		rotation = cameraMath.rotationUp(position, rotation, deltaTime, focalLength);
	}
	if (KeyPress::isKeyCurrentlyDown('k')) {
		rotation = cameraMath.rotationDown(position, rotation, deltaTime, focalLength);
	}

	if (KeyPress::isKeyCurrentlyDown('j')) {
		rotation = cameraMath.rotationLeft(position, rotation, deltaTime, focalLength);
	}
	if (KeyPress::isKeyCurrentlyDown('l')) {
		rotation = cameraMath.rotationRight(position, rotation, deltaTime, focalLength);
	}

	if (KeyPress::isKeyCurrentlyDown('u')) {
		rotation = cameraMath.rotationCounterclockwise(position, rotation, deltaTime, focalLength);
	}
	if (KeyPress::isKeyCurrentlyDown('o')) {
		rotation = cameraMath.rotationClockwise(position, rotation, deltaTime, focalLength);
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
		useCameraFromTimeline();
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

void CameraController::useCameraFromTimeline() {
	const glm::vec3 tmpPosition = getCameraPositionFromTimeline();
	const glm::quat tmpRotation = getCameraRotationFromTimeline();
	const float tmpFocalLength = getCameraFocalLengthFromTimeline();

	cameraMutex.lock();
	focalLength = tmpFocalLength;
	position = tmpPosition;
	rotation = tmpRotation;
	cameraMutex.unlock();
}

glm::vec3 CameraController::getCameraPositionFromTimeline() {
	using namespace glm;

	ValueTree craneActiveUniform = data.getUniform(var("camera_crane_active"));
	ValueTree craneActiveValue = interpolator.getUniformStateFromTimelineData(craneActiveUniform).first;
	const bool craneActive = data.getBoolFromValue(craneActiveValue);

	if (craneActive) {
		ValueTree craneBaseUniform = data.getUniform(var("camera_crane_base"));
		ValueTree craneBaseValue = interpolator.getUniformStateFromTimelineData(craneBaseUniform).first;
		const vec3 craneBase = data.getVec3FromValue(craneBaseValue);

		ValueTree craneLengthUniform = data.getUniform(var("camera_crane_length"));
		ValueTree craneLengthValue = interpolator.getUniformStateFromTimelineData(craneLengthUniform).first;
		const float craneLength = data.getFloatFromValue(craneLengthValue);

		ValueTree cranePhiUniform = data.getUniform(var("camera_crane_phi"));
		ValueTree cranePhiValue = interpolator.getUniformStateFromTimelineData(cranePhiUniform).first;
		const float cranePhi = 2 * pi<float>() * data.getFloatFromValue(cranePhiValue);

		ValueTree craneThetaUniform = data.getUniform(var("camera_crane_theta"));
		ValueTree craneThetaValue = interpolator.getUniformStateFromTimelineData(craneThetaUniform).first;
		const float craneTheta = 2 * pi<float>() * data.getFloatFromValue(craneThetaValue);

		const vec3 craneHeadRelative = craneLength * vec3(sin(craneTheta) * cos(cranePhi), cos(craneTheta), sin(craneTheta) * sin(cranePhi));
		const vec3 craneHeadAbsolute = craneBase + craneHeadRelative;

		return craneHeadAbsolute;

	} else {
		ValueTree positionUniform = data.getUniform(var(cameraPositionName));
		ValueTree positionValue = interpolator.getUniformStateFromTimelineData(positionUniform).first;

		return data.getVec3FromValue(positionValue);
	}
}

glm::quat CameraController::getCameraRotationFromTimeline() {
	using namespace glm;

	ValueTree trackingActiveUniform = data.getUniform(var("camera_tracking_active"));
	ValueTree trackingActiveValue = interpolator.getUniformStateFromTimelineData(trackingActiveUniform).first;
	const bool trackingActive = data.getBoolFromValue(trackingActiveValue);

	if (trackingActive) {
		// also hard coded in camera.glsl
		const float camera_sensor_width = 0.024f;

		ValueTree trackingTargetUniform = data.getUniform(var("camera_tracking_target"));
		ValueTree trackingTargetValue = interpolator.getUniformStateFromTimelineData(trackingTargetUniform).first;
		const vec3 trackingTarget = data.getVec3FromValue(trackingTargetValue);

		ValueTree trackingScreenPosUniform = data.getUniform(var("camera_tracking_screen_pos"));
		ValueTree trackingScreenPosValue = interpolator.getUniformStateFromTimelineData(trackingScreenPosUniform).first;
		const vec2 trackingScreenPos = data.getVec2FromValue(trackingScreenPosValue);

		ValueTree trackingRollUniform = data.getUniform(var("camera_tracking_roll"));
		ValueTree trackingRollValue = interpolator.getUniformStateFromTimelineData(trackingRollUniform).first;
		const float trackingRoll = 2 * pi<float>() * data.getFloatFromValue(trackingRollValue);

		// construct the camera coordinate system in a way that it is only dutch angled if wanted
		vec3 view_direction = normalize(trackingTarget - getCameraPositionFromTimeline());
		vec3 view_right = -cross(view_direction, vec3(0., 1., 0.));

		if (view_right == vec3(0.)) {
			// view_direction parallel to y-axis
			view_right = vec3(1., 0., 0.);
		} else {
			view_right = normalize(view_right);
		}

		// rotate view_right around view_direction
		// from https://en.wikipedia.org/wiki/Rodrigues'_rotation_formula
		// simplified, because view_right and view_direction are perpendicular
		view_right = cos(trackingRoll) * view_right + sin(trackingRoll) * cross(view_direction, view_right);

		vec3 view_up = normalize(cross(view_direction, view_right));
		mat3 rotation_matrix = mat3(-view_right, view_up, -view_direction);


		// rotate in such a way that target is at camera_tracking_screen_pos
		vec2 screen_pos_normalized = (trackingScreenPos - .5f) * vec2(1.f, 9.f/16.f);
		vec2 screen_pos_tangens = screen_pos_normalized / (getCameraFocalLengthFromTimeline() / camera_sensor_width);

		vec2 c_arg = 1.f / sqrt(1.f + square(screen_pos_tangens)); // cos(arctan(arg));
		vec2 s_arg = screen_pos_tangens * c_arg; // sin(arctan(arg));

		// rotation around x axis with arg.y as angle
		mat3 rot_x_axis = mat3(
				1., 0., 0.,
				0., c_arg.y, -s_arg.y,
				0., s_arg.y, c_arg.y
				);

		// rotation around y axis with arg.x as angle
		// transpose to get the left-handed rotation
		mat3 rot_y_axis = transpose(mat3(
					c_arg.x, 0., s_arg.x,
					0., 1., 0.,
					-s_arg.x, 0., c_arg.x
					));

		// do x axis rotation in camera space and y axis rotation in world space
		rotation_matrix = rot_y_axis * rotation_matrix * rot_x_axis;


		return toQuat(rotation_matrix);

	} else {
		ValueTree rotationUniform = data.getUniform(var(cameraRotationName));
		ValueTree rotationValue = interpolator.getUniformStateFromTimelineData(rotationUniform).first;

		return data.getQuatFromValue(rotationValue);
	}
}

glm::quat CameraController::getCameraRotationFromController() {
	if (getHasControl()) {
		return rotation;
	} else {
		return getCameraRotationFromTimeline();
	}
}

float CameraController::getCameraFocalLengthFromTimeline() {
	ValueTree focalLengthUniform = data.getUniform(var(cameraFocalLengthName));
	ValueTree focalLengthValue = interpolator.getUniformStateFromTimelineData(focalLengthUniform).first;
	return data.getFloatFromValue(focalLengthValue);
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
		useCameraFromTimeline();
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
	rotation = cameraMath.mouseMove(position, rotation, deltaTime, focalLength, glm::vec2(deltaMousePos.x, -deltaMousePos.y)); // origin in bottom left corner
	cameraMutex.unlock();

	sendChangeMessage();
}

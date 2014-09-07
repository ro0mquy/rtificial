#include "CameraController.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "CameraMath.h"
#include <glm/glm.hpp>

using namespace glm;

CameraController::CameraController() :
	uniformValue(treeId::uniformStandardValue),
	lastCallback(Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks()))
{
	startTimer(15);
}

var CameraController::getUniformNameToBeControlled() {
	return var("camera_position");
}

bool CameraController::getUseControlledUniform() {
	return true;
}

ValueTree CameraController::getControlledUniformState() {
	return uniformValue;
}

void CameraController::timerCallback() {
	const double tmpLastCallback = lastCallback;
	lastCallback = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
	float deltaTime = lastCallback - tmpLastCallback;

	if (KeyPress::isKeyCurrentlyDown('w')) {
		setPosition(CameraMath::positionForward(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
	if (KeyPress::isKeyCurrentlyDown('s')) {
		setPosition(CameraMath::positionBackward(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}

	if (KeyPress::isKeyCurrentlyDown('a')) {
		setPosition(CameraMath::positionLeft(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
	if (KeyPress::isKeyCurrentlyDown('d')) {
		setPosition(CameraMath::positionRight(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}

	if (KeyPress::isKeyCurrentlyDown('e')) {
		setPosition(CameraMath::positionUp(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
	if (KeyPress::isKeyCurrentlyDown('c')) {
		setPosition(CameraMath::positionDown(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
}

vec3 CameraController::getVec3Position() {
	float x = uniformValue.getProperty(treeId::valueVec3X);
	float y = uniformValue.getProperty(treeId::valueVec3Y);
	float z = uniformValue.getProperty(treeId::valueVec3Z);
	return vec3(x, y, z);
}

vec3 CameraController::getVec3Direction() {
	return vec3(0., 0., -1.);
}

vec3 CameraController::getVec3Up() {
	return vec3(0., 1., 0.);
}

void CameraController::setPosition(vec3 newPosition) {
	uniformValue.setProperty(treeId::valueVec3X, newPosition.x, nullptr);
	uniformValue.setProperty(treeId::valueVec3Y, newPosition.y, nullptr);
	uniformValue.setProperty(treeId::valueVec3Z, newPosition.z, nullptr);
}

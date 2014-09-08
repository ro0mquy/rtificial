#include "CameraController.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"
#include "CameraMath.h"
#include <glm/glm.hpp>

using namespace glm;

CameraController::CameraController() :
	positionValue(treeId::uniformStandardValue),
	directionValue(treeId::uniformStandardValue),
	upValue(treeId::uniformStandardValue),
	lastCallback(Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks()))
{
	directionValue.setProperty(treeId::valueVec3Z, -1., nullptr);
	upValue.setProperty(treeId::valueVec3Y, 1., nullptr);

	startTimer(15);
}

bool CameraController::hasUniformToBeControlled(var& name) {
	String strName = name.toString();
	return strName == "camera_position" ||
		strName == "camera_direction" ||
		strName == "camera_up";
}

bool CameraController::getUseControlledUniform() {
	return true;
}

ValueTree CameraController::getControlledUniformState(var& name) {
	String strName = name.toString();
	if (strName == "camera_position") {
		return positionValue;
	} else if (strName == "camera_direction") {
		return directionValue;
	} else if (strName == "camera_up") {
		return upValue;
	}
	return ValueTree();
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

	if (KeyPress::isKeyCurrentlyDown('i')) {
		setRotation(CameraMath::rotationUp(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
	if (KeyPress::isKeyCurrentlyDown('k')) {
		setRotation(CameraMath::rotationDown(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}

	if (KeyPress::isKeyCurrentlyDown('j')) {
		setRotation(CameraMath::rotationLeft(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
	if (KeyPress::isKeyCurrentlyDown('l')) {
		setRotation(CameraMath::rotationRight(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}

	if (KeyPress::isKeyCurrentlyDown('u')) {
		setRotation(CameraMath::rotationCounterclockwise(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
	if (KeyPress::isKeyCurrentlyDown('o')) {
		setRotation(CameraMath::rotationClockwise(getVec3Position(), getVec3Direction(), getVec3Up(), deltaTime));
	}
}

vec3 CameraController::getVec3Position() {
	float x = positionValue.getProperty(treeId::valueVec3X);
	float y = positionValue.getProperty(treeId::valueVec3Y);
	float z = positionValue.getProperty(treeId::valueVec3Z);
	return vec3(x, y, z);
}

vec3 CameraController::getVec3Direction() {
	float x = directionValue.getProperty(treeId::valueVec3X);
	float y = directionValue.getProperty(treeId::valueVec3Y);
	float z = directionValue.getProperty(treeId::valueVec3Z);
	return vec3(x, y, z);
}

vec3 CameraController::getVec3Up() {
	float x = upValue.getProperty(treeId::valueVec3X);
	float y = upValue.getProperty(treeId::valueVec3Y);
	float z = upValue.getProperty(treeId::valueVec3Z);
	return vec3(x, y, z);
}

void CameraController::setPosition(vec3 newPosition) {
	positionValue.setProperty(treeId::valueVec3X, newPosition.x, nullptr);
	positionValue.setProperty(treeId::valueVec3Y, newPosition.y, nullptr);
	positionValue.setProperty(treeId::valueVec3Z, newPosition.z, nullptr);
}

void CameraController::setRotation(CameraMath::Rotation rotation) {
	vec3 direction = rotation.first;
	directionValue.setProperty(treeId::valueVec3X, direction.x, nullptr);
	directionValue.setProperty(treeId::valueVec3Y, direction.y, nullptr);
	directionValue.setProperty(treeId::valueVec3Z, direction.z, nullptr);

	vec3 up = rotation.second;
	upValue.setProperty(treeId::valueVec3X, up.x, nullptr);
	upValue.setProperty(treeId::valueVec3Y, up.y, nullptr);
	upValue.setProperty(treeId::valueVec3Z, up.z, nullptr);
}

#include "CameraController.h"
#include "TimelineData.h"
#include "TreeIdentifiers.h"

CameraController::CameraController() :
	uniformValue(treeId::uniformStandardValue),
	lastCallback(Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks())),
	movementSpeed(.5)
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
	const double dtime = lastCallback - tmpLastCallback;

	if (KeyPress::isKeyCurrentlyDown('d')) {
		float vec3X = uniformValue.getProperty(treeId::valueVec3X);
		vec3X += movementSpeed * dtime;
		uniformValue.setProperty(treeId::valueVec3X, vec3X, nullptr);
	}
	if (KeyPress::isKeyCurrentlyDown('a')) {
		float vec3X = uniformValue.getProperty(treeId::valueVec3X);
		vec3X -= movementSpeed * dtime;
		uniformValue.setProperty(treeId::valueVec3X, vec3X, nullptr);
	}

	if (KeyPress::isKeyCurrentlyDown('e')) {
		float vec3Y = uniformValue.getProperty(treeId::valueVec3Y);
		vec3Y += movementSpeed * dtime;
		uniformValue.setProperty(treeId::valueVec3Y, vec3Y, nullptr);
	}
	if (KeyPress::isKeyCurrentlyDown('c')) {
		float vec3Y = uniformValue.getProperty(treeId::valueVec3Y);
		vec3Y -= movementSpeed * dtime;
		uniformValue.setProperty(treeId::valueVec3Y, vec3Y, nullptr);
	}

	if (KeyPress::isKeyCurrentlyDown('s')) {
		float vec3Z = uniformValue.getProperty(treeId::valueVec3Z);
		vec3Z += movementSpeed * dtime;
		uniformValue.setProperty(treeId::valueVec3Z, vec3Z, nullptr);
	}
	if (KeyPress::isKeyCurrentlyDown('w')) {
		float vec3Z = uniformValue.getProperty(treeId::valueVec3Z);
		vec3Z -= movementSpeed * dtime;
		uniformValue.setProperty(treeId::valueVec3Z, vec3Z, nullptr);
	}
}

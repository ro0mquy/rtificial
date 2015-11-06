#include "DraggableQuaternion.h"

#include <Timeline/TimelineData.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

static const float rotationSpeed = .01f;

DraggableQuaternion::DraggableQuaternion() {
}

DraggableQuaternion::DraggableQuaternion(const quat& quaternionToUse) :
	quaternion(quaternionToUse)
{
}

void DraggableQuaternion::mouseDown(Point<float> mousePos) {
	lastMousePos = mousePos;
}

void DraggableQuaternion::mouseDrag(Point<float> mousePos) {
	quaternion *= rotationFromMove(lastMousePos, mousePos);
	lastMousePos = mousePos;
}

quat& DraggableQuaternion::getQuaternion() {
	return quaternion;
}

quat DraggableQuaternion::rotationFromMove(const Point<float> last, const Point<float> current) {
	const Point<float> delta = current - last;
	const float rotationAngleX = -rotationSpeed * delta.x;
	const float rotationAngleY = -rotationSpeed * delta.y;

	// get current camera rotation to rotate the rotation vectors
	TimelineData& data = TimelineData::getTimelineData();
	const ValueTree cameraRotationValue = data.getInterpolator().getCurrentUniformState(var("camera_rotation")).first;
	const quat cameraRotationQuat = data.getQuatFromValue(cameraRotationValue);

	// rotate roation axis
	const vec3 xAxis = rotate(cameraRotationQuat, vec3(1., 0., 0.));
	quat quatRot = angleAxis(rotationAngleY, xAxis);

	const vec3 yAxis = rotate(cameraRotationQuat, vec3(0., 1., 0.));
	quatRot *= angleAxis(rotationAngleX, yAxis);

	return quatRot;
}

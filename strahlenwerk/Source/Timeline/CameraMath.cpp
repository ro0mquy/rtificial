#include "CameraMath.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

static const float speedAdjustmentFactor = .75;
static const float rotationMouseFactor = .025;

vec3 CameraMath::positionForward(vec3 position, quat rotation, float dtime) {
	// only move in xz-plane
	// quat * vec rotates the vec with the quat
	vec3 dir = rotation * vec3(0., 0., -1.);
	dir.y = 0.;
	dir = normalize(dir);
	return position + dtime * movementSpeed * dir;
}

vec3 CameraMath::positionBackward(vec3 position, quat rotation, float dtime) {
	// only move in xz-plane
	// quat * vec rotates the vec with the quat
	vec3 dir = rotation * vec3(0., 0., -1.);
	dir.y = 0.;
	dir = normalize(dir);
	return position - dtime * movementSpeed * dir;
}

vec3 CameraMath::positionLeft(vec3 position, quat rotation, float dtime) {
	// only move in xz-plane
	// quat * vec rotates the vec with the quat
	vec3 dir = rotation * vec3(1., 0., 0.);
	dir.y = 0.;
	dir = normalize(dir);
	return position - dtime * movementSpeed * dir;
}

vec3 CameraMath::positionRight(vec3 position, quat rotation, float dtime) {
	// only move in xz-plane
	// quat * vec rotates the vec with the quat
	vec3 dir = rotation * vec3(1., 0., 0.);
	dir.y = 0.;
	dir = normalize(dir);
	return position + dtime * movementSpeed * dir;
}

vec3 CameraMath::positionUp(vec3 position, quat /*rotation*/, float dtime) {
	// always just change the y component
	position.y += dtime * movementSpeed;
	return position;
}

vec3 CameraMath::positionDown(vec3 position, quat /*rotation*/, float dtime) {
	// always just change the y component
	position.y -= dtime * movementSpeed;
	return position;
}

quat CameraMath::rotationUp(vec3 /*position*/, quat rotation, float dtime) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngle = dtime * rotationSpeed;
	return rotation * angleAxis(rotationAngle, vec3(1., 0., 0.));
}

quat CameraMath::rotationDown(vec3 /*position*/, quat rotation, float dtime) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngle = - dtime * rotationSpeed;
	return rotation * angleAxis(rotationAngle, vec3(1., 0., 0.));
}

quat CameraMath::rotationLeft(vec3 /*position*/, quat rotation, float dtime) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngle = dtime * rotationSpeed;
	return angleAxis(rotationAngle, vec3(0., 1., 0.)) * rotation;
}

quat CameraMath::rotationRight(vec3 /*position*/, quat rotation, float dtime) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngle = - dtime * rotationSpeed;
	return angleAxis(rotationAngle, vec3(0., 1., 0.)) * rotation;
}

quat CameraMath::rotationCounterclockwise(vec3 /*position*/, quat rotation, float dtime) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngle = - dtime * rotationSpeed;
	return rotation * angleAxis(rotationAngle, vec3(0., 0., -1.));
}

quat CameraMath::rotationClockwise(vec3 /*position*/, quat rotation, float dtime) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngle = dtime * rotationSpeed;
	return rotation * angleAxis(rotationAngle, vec3(0., 0., -1.));
}

quat CameraMath::mouseMove(vec3 /*position*/, quat rotation, float dtime, vec2 dmouse) {
	// rotation * newAxis for rotation in local coordinate system
	// newAxis * rotation for rotation in global coordinate system
	const float rotationAngleX = - dtime * rotationSpeed * rotationMouseFactor * dmouse.x;
	const float rotationAngleY =   dtime * rotationSpeed * rotationMouseFactor * dmouse.y;
	return angleAxis(rotationAngleX, vec3(0., 1., 0.)) * rotation * angleAxis(rotationAngleY, vec3(1., 0., 0.));
}

void CameraMath::movementSpeedTuning(float dscroll) {
	movementSpeed = clamp(movementSpeed * pow(speedAdjustmentFactor, -dscroll), .001f, 1000.f);
}

void CameraMath::rotationSpeedTuning(float dscroll) {
	rotationSpeed *= clamp(pow(speedAdjustmentFactor, -dscroll), .0001f, 100.f);
}

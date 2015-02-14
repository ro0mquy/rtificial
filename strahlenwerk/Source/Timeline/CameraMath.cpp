#include "CameraMath.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

static const float movementSpeed = 5.;
static const float rotationSpeed = 6.28318530718 /*TAU*/ / 4.;

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

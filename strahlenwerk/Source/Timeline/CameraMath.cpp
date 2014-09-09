#include "CameraMath.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace glm;

static const float movementSpeed = 5.;
static const float rotationSpeed = 90.;

CameraMath::CameraMath()
{
}

vec3 CameraMath::positionForward(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// project the direction into the xz plane and don't use the y component
	vec3 xzRotation = direction;
	xzRotation.y = 0.;
	xzRotation = normalize(xzRotation);
	position += dtime * movementSpeed * xzRotation;
	return position;
}

vec3 CameraMath::positionBackward(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// project the direction into the xz plane and don't use the y component
	vec3 xzRotation = direction;
	xzRotation.y = 0.;
	xzRotation = normalize(xzRotation);
	position -= dtime * movementSpeed * xzRotation;
	return position;
}

vec3 CameraMath::positionLeft(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// use the cross product of direction and the xz-projection of the direction
	// (x, y, z) x (x, 0, z) = y * (z, 0, -x)
	// so the vec3(z, 0, -x) is the a bit simplified direction
	vec3 dirCrossXzDir = vec3(direction.z, 0., -direction.x);
	dirCrossXzDir = normalize(dirCrossXzDir);
	position += dtime * movementSpeed * dirCrossXzDir;
	return position;
}

vec3 CameraMath::positionRight(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// use the cross product of direction and the xz-projection of the direction
	vec3 dirCrossXzDir = vec3(direction.z, 0., -direction.x);
	dirCrossXzDir = normalize(dirCrossXzDir);
	position -= dtime * movementSpeed * dirCrossXzDir;
	return position;
}

vec3 CameraMath::positionUp(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// always just change the y component
	position.y += dtime * movementSpeed;
	return position;
}

vec3 CameraMath::positionDown(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// always just change the y component
	position.y -= dtime * movementSpeed;
	return position;
}

CameraMath::Rotation CameraMath::rotationUp(vec3 position, vec3 direction, vec3 up, float dtime) {
	// use the cross product of direction and the xz-projection of the direction
	// (x, y, z) x (x, 0, z) = y * (z, 0, -x)
	// so the vec3(z, 0, -x) is the a bit simplified direction
	vec3 dirCrossXzDir = vec3(direction.z, 0., -direction.x);
	//dirCrossXzDir = normalize(dirCrossXzDir);

	const float rotationAngle = - dtime * rotationSpeed;

	vec3 newDirection = rotate(direction, rotationAngle, dirCrossXzDir);
	vec3 newUp = rotate(up, rotationAngle, dirCrossXzDir);

	return CameraMath::Rotation(newDirection, newUp);
}

CameraMath::Rotation CameraMath::rotationDown(vec3 position, vec3 direction, vec3 up, float dtime) {
	// use the cross product of direction and the xz-projection of the direction
	// (x, y, z) x (x, 0, z) = y * (z, 0, -x)
	// so the vec3(z, 0, -x) is the a bit simplified direction
	vec3 dirCrossXzDir = vec3(direction.z, 0., -direction.x);
	//dirCrossXzDir = normalize(dirCrossXzDir);

	const float rotationAngle = dtime * rotationSpeed;

	vec3 newDirection = rotate(direction, rotationAngle, dirCrossXzDir);
	vec3 newUp = rotate(up, rotationAngle, dirCrossXzDir);

	return CameraMath::Rotation(newDirection, newUp);
}

CameraMath::Rotation CameraMath::rotationLeft(vec3 position, vec3 direction, vec3 up, float dtime) {
	const float rotationAngle = dtime * rotationSpeed;

	vec3 newDirection = rotateY(direction, rotationAngle);
	vec3 newUp = rotateY(up, rotationAngle);

	return CameraMath::Rotation(newDirection, newUp);
}

CameraMath::Rotation CameraMath::rotationRight(vec3 position, vec3 direction, vec3 up, float dtime) {
	const float rotationAngle = - dtime * rotationSpeed;

	vec3 newDirection = rotateY(direction, rotationAngle);
	vec3 newUp = rotateY(up, rotationAngle);

	return CameraMath::Rotation(newDirection, newUp);
}

CameraMath::Rotation CameraMath::rotationCounterclockwise(vec3 position, vec3 direction, vec3 up, float dtime) {
	const float rotationAngle = - dtime * rotationSpeed;
	vec3 newUp = rotate(up, rotationAngle, direction);
	return CameraMath::Rotation(direction, newUp);
}

CameraMath::Rotation CameraMath::rotationClockwise(vec3 position, vec3 direction, vec3 up, float dtime) {
	const float rotationAngle = dtime * rotationSpeed;
	vec3 newUp = rotate(up, rotationAngle, direction);
	return CameraMath::Rotation(direction, newUp);
}

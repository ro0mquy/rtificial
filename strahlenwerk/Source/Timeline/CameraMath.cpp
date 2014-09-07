#include "CameraMath.h"
#include <glm/glm.hpp>

using namespace glm;

static const float movementSpeed = .5;

CameraMath::CameraMath()
{
}

vec3 CameraMath::positionForward(vec3 position, vec3 direction, vec3 up, float dtime) {
	vec3 xzDirection = direction;
	xzDirection.y = 0.;
	xzDirection = normalize(xzDirection);
	position += dtime * movementSpeed * xzDirection;
	return position;
}

vec3 CameraMath::positionBackward(vec3 position, vec3 direction, vec3 up, float dtime) {
	vec3 xzDirection = direction;
	xzDirection.y = 0.;
	xzDirection = normalize(xzDirection);
	position -= dtime * movementSpeed * xzDirection;
	return position;
}

vec3 CameraMath::positionLeft(vec3 position, vec3 direction, vec3 up, float dtime) {
	vec3 dirCrossXzDir = vec3(direction.z, 0., -direction.x);
	dirCrossXzDir = normalize(dirCrossXzDir);
	position += dtime * movementSpeed * dirCrossXzDir;
	return position;
}

vec3 CameraMath::positionRight(vec3 position, vec3 direction, vec3 up, float dtime) {
	vec3 dirCrossXzDir = vec3(direction.z, 0., -direction.x);
	dirCrossXzDir = normalize(dirCrossXzDir);
	position -= dtime * movementSpeed * dirCrossXzDir;
	return position;
}

vec3 CameraMath::positionUp(vec3 position, vec3 direction, vec3 up, float dtime) {
	position.y += dtime * movementSpeed;
	return position;
}

vec3 CameraMath::positionDown(vec3 position, vec3 direction, vec3 up, float dtime) {
	position.y -= dtime * movementSpeed;
	return position;
}

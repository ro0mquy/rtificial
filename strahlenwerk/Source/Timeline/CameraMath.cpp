#include "CameraMath.h"
#include <glm/glm.hpp>

using namespace glm;

static const float movementSpeed = .5;

CameraMath::CameraMath()
{
}

vec3 CameraMath::positionForward(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// project the direction into the xz plane and don't use the y component
	vec3 xzDirection = direction;
	xzDirection.y = 0.;
	xzDirection = normalize(xzDirection);
	position += dtime * movementSpeed * xzDirection;
	return position;
}

vec3 CameraMath::positionBackward(vec3 position, vec3 direction, vec3 /*up*/, float dtime) {
	// project the direction into the xz plane and don't use the y component
	vec3 xzDirection = direction;
	xzDirection.y = 0.;
	xzDirection = normalize(xzDirection);
	position -= dtime * movementSpeed * xzDirection;
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

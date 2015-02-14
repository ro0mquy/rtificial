#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <glm/fwd.hpp>
#include <utility>

class CameraMath {
	public:
		glm::vec3 positionForward(glm::vec3 position, glm::quat rotation, float dtime);
		glm::vec3 positionBackward(glm::vec3 position, glm::quat rotation, float dtime);

		glm::vec3 positionLeft(glm::vec3 position, glm::quat rotation, float dtime);
		glm::vec3 positionRight(glm::vec3 position, glm::quat rotation, float dtime);

		glm::vec3 positionUp(glm::vec3 position, glm::quat rotation, float dtime);
		glm::vec3 positionDown(glm::vec3 position, glm::quat rotation, float dtime);

		glm::quat rotationUp(glm::vec3 position, glm::quat rotation, float dtime);
		glm::quat rotationDown(glm::vec3 position, glm::quat rotation, float dtime);

		glm::quat rotationLeft(glm::vec3 position, glm::quat rotation, float dtime);
		glm::quat rotationRight(glm::vec3 position, glm::quat rotation, float dtime);

		glm::quat rotationCounterclockwise(glm::vec3 position, glm::quat rotation, float dtime);
		glm::quat rotationClockwise(glm::vec3 position, glm::quat rotation, float dtime);
};

#endif // CAMERAMATH_H

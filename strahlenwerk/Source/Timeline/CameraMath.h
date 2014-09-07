#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <glm/fwd.hpp>

class CameraMath {
	public:
		CameraMath();

		static glm::vec3 positionForward(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static glm::vec3 positionBackward(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static glm::vec3 positionLeft(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static glm::vec3 positionRight(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static glm::vec3 positionUp(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static glm::vec3 positionDown(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
};

#endif // CAMERAMATH_H

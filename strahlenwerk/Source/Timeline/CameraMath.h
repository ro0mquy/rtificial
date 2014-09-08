#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <glm/fwd.hpp>
#include <utility>

class CameraMath {
	public:
		CameraMath();

		//typedef std::pair<glm::vec3, glm::vec3> Rotation;
		using Rotation = std::pair<glm::vec3, glm::vec3>;

		static glm::vec3 positionForward(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static glm::vec3 positionBackward(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static glm::vec3 positionLeft(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static glm::vec3 positionRight(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static glm::vec3 positionUp(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static glm::vec3 positionDown(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static Rotation rotationUp(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static Rotation rotationDown(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static Rotation rotationLeft(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static Rotation rotationRight(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);

		static Rotation rotationCounterclockwise(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
		static Rotation rotationClockwise(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float dtime);
};

#endif // CAMERAMATH_H

#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <glm/fwd.hpp>

class CameraMath {
	public:
		glm::vec3 positionForward(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);
		glm::vec3 positionBackward(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);

		glm::vec3 positionLeft(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);
		glm::vec3 positionRight(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);

		glm::vec3 positionUp(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);
		glm::vec3 positionDown(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);

		glm::quat rotationUp(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);
		glm::quat rotationDown(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);

		glm::quat rotationLeft(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);
		glm::quat rotationRight(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);

		glm::quat rotationCounterclockwise(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);
		glm::quat rotationClockwise(glm::vec3 position, glm::quat rotation, float dtime, float focalLength);

		glm::quat mouseMove(glm::vec3 position, glm::quat rotation, float dtime, float focalLength, glm::vec2 dmouse);

		float focalLengthTuning(float focalLength, float dscroll);

		void movementSpeedTuning(float dscroll);
		void rotationSpeedTuning(float dscroll);

	private:
		float movementSpeed = 5.;
		float rotationSpeed = 6.28318530718 /*TAU*/ / 4. * .022 /* default focal length */;
};

#endif // CAMERAMATH_H

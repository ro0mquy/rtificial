#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <juce>
#include <glm/fwd.hpp>
#include "CameraMath.h"

class CameraController :
	private Timer
{
	public:
		CameraController();

		bool hasUniformToBeControlled(var& name);
		bool getUseControlledUniform();
		ValueTree getControlledUniformState(var& name);

		void timerCallback() override;

	private:
		glm::vec3 getVec3Position();
		glm::vec3 getVec3Direction();
		glm::vec3 getVec3Up();

		void setPosition(glm::vec3 newPosition);
		void setRotation(CameraMath::Rotation rotation);

		ValueTree positionValue;
		ValueTree directionValue;
		ValueTree upValue;
		float timeValue;
		double lastCallback;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraController)
};

#endif // CAMERACONTROLLER_H

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <juce>
#include <glm/fwd.hpp>

class CameraController :
	private Timer
{
	public:
		CameraController();

		var getUniformNameToBeControlled();
		bool getUseControlledUniform();
		ValueTree getControlledUniformState();

		void timerCallback() override;

	private:
		glm::vec3 getVec3Position();
		glm::vec3 getVec3Direction();
		glm::vec3 getVec3Up();

		void setPosition(glm::vec3 newPosition);

		ValueTree uniformValue;
		double lastCallback;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraController)
};

#endif // CAMERACONTROLLER_H

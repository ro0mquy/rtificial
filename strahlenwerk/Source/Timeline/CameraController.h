#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <juce>

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
		ValueTree uniformValue;
		double lastCallback;

		float movementSpeed;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraController)
};

#endif // CAMERACONTROLLER_H

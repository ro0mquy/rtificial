#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <juce>
#include <mutex>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Timeline/SpecialUniformController.h>
#include "CameraMath.h"

class Interpolator;

class CameraController :
	public SpecialUniformController,
	public ChangeBroadcaster,
	private KeyListener,
	private MouseListener,
	private AsyncUpdater,
	private Timer,
	private ApplicationCommandManagerListener
{
	public:
		CameraController(TimelineData& data_, Interpolator& interpolator_);
		~CameraController();

		static CameraController* globalCameraController;

		bool shouldAddUniformToTimlineData(String& uniformName) override;
		bool wantControlUniform(String& uniformName) override;
		std::pair<ValueTree, bool> getUniformState(String& uniformName) override;

		bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
		bool keyStateChanged(bool isKeyPressed, Component* originatingComponent) override;

		void mouseMove(const MouseEvent& event) override;
		void mouseDown(const MouseEvent& event) override;
		void mouseDrag(const MouseEvent& event) override;
		void mouseUp(const MouseEvent& event) override;
		void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override;

		void handleAsyncUpdate() override;
		void timerCallback() override;

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

		void setKeyframeAtCurrentPosition();
		void getCameraFromCurrentPosition();

		bool getHasControl();
		void setHasControl(const bool shouldHaveControl);
		void takeOverControl();
		void releaseControl();

		enum CommandIDs {
			playPauseWithAnimation = 0x4f81da00,
			playPauseWithoutAnimation,
			setKeyframe,
			resetCameraPosition,
			resetCameraRotation,
			increaseMovementSpeed,
			decreaseMovementSpeed,
			increaseRotationSpeed,
			decreaseRotationSpeed,
		};

	private:
		Interpolator& interpolator;
		std::mutex cameraMutex;
		CameraMath cameraMath;
		double lastCallbackTime;
		double lastMouseMoveTime;
		Point<int> originalMousePos;
		float focalLength = .0f;
		glm::vec3 position;
		glm::quat rotation;
		bool hasControl = false;
		bool shiftDraggingActive = false;

		const String cameraFocalLengthName;
		const String cameraPositionName;
		const String cameraRotationName;
		const String cameraCraneActiveName;
		const String cameraTrackingActiveName;

		const String spectatormodeActiveName;
		const String spectatormodePositionName;
		const String spectatormodeRotationName;
		const String spectatormodeFocalLengthName;

		void startTimerCallback();
		void stopTimerCallback();
		void startMouseDragging();
		void stopMouseDragging();
		void handleMouseDragging();

		static const int timerInterval = 15;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraController)
};

#endif // CAMERACONTROLLER_H

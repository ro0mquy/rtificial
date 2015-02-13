#ifndef SPECIALUNIFORMCONTROLLER_H
#define SPECIALUNIFORMCONTROLLER_H

#include <juce>
#include <mutex>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "CameraMath.h"

class TimelineData;
class Interpolator;

static const int timerInterval = 15;

class SpecialUniformController {
	public:
		SpecialUniformController(TimelineData& data_) : data(data_) {}
		virtual ~SpecialUniformController() {}

		// gets called when new uniforms should be added to TimelineData
		// when this function returns false, it won't get added
		virtual bool shouldAddUniformToTimlineData(String& /*uniformName*/) { return true; }

		// gets called when the state of a uniform is needed
		// controller gets control when true is returned
		virtual bool wantControlUniform(String& uniformName) = 0;

		// should return the actual value of the uniform
		virtual std::pair<ValueTree, bool> getUniformState(String& uniformName) = 0;

	protected:
		TimelineData& data;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpecialUniformController)
};

class TimeController :
	public SpecialUniformController
{
	public:
		TimeController(TimelineData& data_) : SpecialUniformController(data_) {}
		bool shouldAddUniformToTimlineData(String& uniformName) override;
		bool wantControlUniform(String& uniformName) override;
		std::pair<ValueTree, bool> getUniformState(String& uniformName) override;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeController)
};

class CameraController :
	public SpecialUniformController,
	public ChangeBroadcaster,
	private KeyListener,
	private AsyncUpdater,
	private Timer,
	private ApplicationCommandManagerListener
{
	public:
		CameraController(TimelineData& data_, Interpolator& interpolator_);
		~CameraController();

		static CameraController* globalCameraController;

		bool wantControlUniform(String& uniformName) override;
		std::pair<ValueTree, bool> getUniformState(String& uniformName) override;

		bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
		bool keyStateChanged(bool isKeyPressed, Component* originatingComponent) override;

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
		};

	private:
		Interpolator& interpolator;
		std::mutex cameraMutex;
		CameraMath cameraMath;
		double lastCallback;
		glm::vec3 position;
		glm::quat rotation;
		bool hasControl;

		const String cameraPositionName;
		const String cameraRotationName;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraController)
};

#endif // SPECIALUNIFORMCONTROLLER_H

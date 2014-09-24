#ifndef SPECIALUNIFORMCONTROLLER_H
#define SPECIALUNIFORMCONTROLLER_H

#include <juce>
#include "Interpolator.h"

class TimelineData;

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
		virtual Interpolator::UniformState getUniformState(String& uniformName) = 0;

	protected:
		TimelineData& data;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpecialUniformController)
};

class TimeController :
	public SpecialUniformController
{
	public:
		using SpecialUniformController::SpecialUniformController;
		bool shouldAddUniformToTimlineData(String& uniformName) override;
		bool wantControlUniform(String& uniformName) override;
		Interpolator::UniformState getUniformState(String& uniformName) override;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeController)
};

class CameraController :
	public SpecialUniformController
{
	public:
		using SpecialUniformController::SpecialUniformController;
		bool wantControlUniform(String& uniformName) override;
		Interpolator::UniformState getUniformState(String& uniformName) override;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraController)
};

#endif // SPECIALUNIFORMCONTROLLER_H

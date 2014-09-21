#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <juce>
#include "CameraController.h"

class TimelineData;

class Interpolator {
	public:
		Interpolator(TimelineData& data_);

		std::pair<ValueTree, bool> getCurrentUniformValue(ValueTree uniformData);
		std::pair<ValueTree, bool> getCurrentUniformValue(const var& name);

	private:
		std::pair<ValueTree, bool> calculateInterpolatedValue(ValueTree sequence, const float relativeCurrentTime);

		// functions for interpolation methods
		std::pair<ValueTree, bool> interpolationMethodStep(ValueTree sequence, const float currentTime);
		std::pair<ValueTree, bool> interpolationMethodLinear(ValueTree sequence, const float currentTime);

		TimelineData& data;
		CameraController cameraController;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

#endif // INTERPOLATOR_H

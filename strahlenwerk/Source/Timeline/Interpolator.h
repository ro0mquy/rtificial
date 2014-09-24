#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <juce>
#include "SpecialUniformController.h"

class TimelineData;

class Interpolator {
	public:
		Interpolator(TimelineData& data_);

		// typedef: pair.first is curent value, pair.second is bool for if on keyframe
		using UniformState = std::pair<ValueTree, bool>;

		bool shouldAddUniformToTimlineData(String uniformName);

		UniformState getCurrentUniformState(ValueTree uniformData);
		UniformState getCurrentUniformState(const var& name);

		UniformState getUniformStateFromTimelineData(ValueTree uniformData);

	private:
		UniformState calculateInterpolatedState(ValueTree sequence, const float relativeCurrentTime);

		// functions for interpolation methods
		UniformState interpolationMethodStep(ValueTree sequence, const float currentTime);
		UniformState interpolationMethodLinear(ValueTree sequence, const float currentTime);
		UniformState interpolationMethodCcrSpline(ValueTree sequence, const float currentTime);

		TimelineData& data;
		OwnedArray<SpecialUniformController> specialUniformControllers;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

#endif // INTERPOLATOR_H

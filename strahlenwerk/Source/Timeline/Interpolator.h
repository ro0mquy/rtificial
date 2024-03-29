#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <juce>
#include <utility>
#include "SpecialUniformController.h"

class TimelineData;

class Interpolator {
	public:
		Interpolator(TimelineData& data_);

		// typedef: pair.first is current value, pair.second is bool for if on keyframe
		using UniformState = std::pair<ValueTree, bool>;

		bool shouldAddUniformToTimlineData(String uniformName);

		UniformState getCurrentUniformState(ValueTree uniformData);
		UniformState getCurrentUniformState(const var& name);
		UniformState getCurrentUniformState(const var& name, ValueTree defaultState);
		UniformState getCurrentUniformStateWithSequence(ValueTree sequenceData);

		UniformState getUniformStateFromTimelineData(ValueTree uniformData);

		UniformState calculateInterpolatedState(ValueTree sequence, const int relativeCurrentTime);

	private:
		// functions for interpolation methods
		UniformState interpolationMethodStep(ValueTree sequence, const int currentTime);
		UniformState interpolationMethodLinear(ValueTree sequence, const int currentTime);
		UniformState interpolationMethodCcrSpline(ValueTree sequence, const int currentTime);

		std::tuple<ValueTree, const int, const bool, const bool> wrappedGetKeyframe(ValueTree sequence, const int index, const int numKeyframes, const bool useStdAtStart, const bool useStdAtEnd) const;
		static float easingRemapping(const float mixT, const bool easeAwayP1, const bool easeTowardP2);

		TimelineData& data;
		OwnedArray<SpecialUniformController> specialUniformControllers;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

#endif // INTERPOLATOR_H

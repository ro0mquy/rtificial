#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <juce>

class Data;

class Interpolator {
	public:
		Interpolator();

		void updateUniformState(ValueTree uniformData);
		void updateUniformState(const var& name);
		void updateAllUniformStates();

	private:
		Data& data;
		ValueTree uniformStatesArray;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};

#endif // INTERPOLATOR_H

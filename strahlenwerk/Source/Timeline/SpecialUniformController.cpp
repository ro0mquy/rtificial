#include "SpecialUniformController.h"

#include <AudioManager.h>
#include "TimelineData.h"
#include "Interpolator.h"
#include "TreeIdentifiers.h"

bool TimeController::shouldAddUniformToTimlineData(String& uniformName) {
	return uniformName != "time";
}

bool TimeController::wantControlUniform(String& uniformName) {
	return uniformName == "time";
}

Interpolator::UniformState TimeController::getUniformState(String& /*uniformName*/) {
	// time gets stored in milli beats, return normal beats
	const float currentTime = AudioManager::getAudioManager().getTime() / 1000.;
	ValueTree timeValue(treeId::controlledValue);
	data.setFloatToValue(timeValue, currentTime, false);
	return Interpolator::UniformState(timeValue, false);
}

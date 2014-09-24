#include "SpecialUniformController.h"
#include <AudioManager.h>
#include "TimelineData.h"
#include "TreeIdentifiers.h"

bool TimeController::shouldAddUniformToTimlineData(String& uniformName) {
	return uniformName != "time";
}

bool TimeController::wantControlUniform(String& uniformName) {
	return uniformName == "time";
}

Interpolator::UniformState TimeController::getUniformState(String& /*uniformName*/) {
	const float currentTime = AudioManager::getAudioManager().getTimeInBeats();
	ValueTree timeValue(treeId::controlledValue);
	data.setFloatToValue(timeValue, currentTime);
	return Interpolator::UniformState(timeValue, false);
}


bool CameraController::wantControlUniform(String& uniformName) {
	return false;
}

#include <glm/glm.hpp>
Interpolator::UniformState CameraController::getUniformState(String& uniformName) {
	return Interpolator::UniformState(ValueTree(), false);
}

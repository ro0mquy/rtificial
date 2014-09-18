#include "AudioManager.h"

AudioManager::AudioManager() {
	formatManager.registerBasicFormats();
	// no input, stereo output
	deviceManager.initialiseWithDefaultDevices(0, 2);

	deviceManager.addAudioCallback(&player);
	player.setSource(&transportSource);
}

AudioManager::~AudioManager() {
	transportSource.setSource(nullptr);
	player.setSource(nullptr);
	deviceManager.removeAudioCallback(&player);
}

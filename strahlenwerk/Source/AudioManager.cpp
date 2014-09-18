#include "AudioManager.h"

AudioManager::AudioManager() :
	readAheadThread("Audio Read Ahead Thread")
{
	formatManager.registerBasicFormats();
	// no input, stereo output
	deviceManager.initialiseWithDefaultDevices(0, 2);

	readAheadThread.startThread(3); // priority 3

	deviceManager.addAudioCallback(&player);
	player.setSource(&transportSource);
}

AudioManager::~AudioManager() {
	transportSource.setSource(nullptr);
	player.setSource(nullptr);
	deviceManager.removeAudioCallback(&player);
}

void AudioManager::loadFile(const File& audioFile) {
	// unload the previous file source and delete it
	transportSource.stop();
	transportSource.setSource(nullptr);
	currentFileSource = nullptr;

	AudioFormatReader* const reader = formatManager.createReaderFor(audioFile);
	if(reader != nullptr) {
		currentFileSource = new AudioFormatReaderSource(reader, true);

		transportSource.setSource(currentFileSource, 32768, &readAheadThread);
	}
}

#include "AudioManager.h"

AudioManager::AudioManager() :
	readAheadThread("Audio Read Ahead Thread")
{
	formatManager.registerBasicFormats();
	// no input, stereo output
	deviceManager.initialiseWithDefaultDevices(0, 2);

	// wenn man die buffergröße nicht auf etwas über 1024
	// setzt bekommt man hier aus seiner musik besten
	// electro-glitch gemacht (plus extra bonus lüftergeräusche)
	//  wer's mag einfach auskommentieren!
	AudioDeviceManager::AudioDeviceSetup deviceSetup;
	deviceManager.getAudioDeviceSetup(deviceSetup);
	deviceSetup.bufferSize = 1152;
	deviceManager.setAudioDeviceSetup(deviceSetup, true);

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

		transportSource.setSource(currentFileSource, 1 << 18, &readAheadThread);
	}
}

void AudioManager::togglePlayPause() {
	if(transportSource.isPlaying()) {
		transportSource.stop();
	} else {
		transportSource.start();
	}
}

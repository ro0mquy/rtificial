#include "AudioManager.h"
#include "StrahlenwerkApplication.h"

AudioManager::AudioManager() :
	transportSource(*this),
	readAheadThread("Audio Read Ahead Thread"),
	thumbnailCache(1), // actually we don't need caching
	thumbnail(512, formatManager, thumbnailCache)
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

AudioManager& AudioManager::getAudioManager() {
	return StrahlenwerkApplication::getInstance()->getAudioManager();
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

		thumbnail.setSource(new FileInputSource(audioFile));
	}
}

void AudioManager::loadEnvelopes(const File& envelopeFile) {
	if(envelopeFile.exists()) {
		auto stream = envelopeFile.createInputStream();
		envelopeData = std::unique_ptr<float>(new float[stream->getTotalLength() / sizeof(float)]);
		envelopesSize = stream->getTotalLength() / sizeof(float);
		stream->read(&(*envelopeData), stream->getTotalLength());
		delete stream;
	}
}

void AudioManager::togglePlayPause() {
	if(transportSource.isPlaying()) {
		transportSource.stop();
	} else {
		transportSource.start();
	}
}

float AudioManager::getTimeInBeats() {
	return transportSource.getCurrentPosition() * (getBpm() / 60.);
}

void AudioManager::setTime(float newTimeInBeats) {
	transportSource.setPosition(newTimeInBeats / (getBpm() / 60.));
	sendChangeMessage();
}

float* AudioManager::getCurrentEnvelopes() {
	if(envelopeData == nullptr) {
		return nullptr;
	} else {
		const int index = int(transportSource.getCurrentPosition() * 44100) / 256 * 32;
		if(index < envelopesSize) {
			return &(&(*envelopeData))[index];
		} else {
			return nullptr;
		}
	}
}

AudioThumbnail& AudioManager::getThumbnail() {
	return thumbnail;
}

int AudioManager::getBpm() {
	return 118;
}


AudioManager::AudioTransportSourceWithCallback::AudioTransportSourceWithCallback(AudioManager& audioManager_) :
	audioManager(audioManager_)
{
}

void AudioManager::AudioTransportSourceWithCallback::getNextAudioBlock (const AudioSourceChannelInfo& info) {
	AudioTransportSource::getNextAudioBlock(info);
	if (isPlaying()) {
		audioManager.sendChangeMessage();
	}
}

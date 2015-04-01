#include "AudioManager.h"
#include "StrahlenwerkApplication.h"
#include <MainWindow.h>
#include <Timeline/SpecialUniformController.h>
#include <PropertyNames.h>

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

	PropertySet& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool muted = properties.getBoolValue(PropertyNames::AudioMuted);
	transportSource.setGain(muted ? 0.0 : 1.0);

	MainWindow::getApplicationCommandManager().addListener(this);
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
	sendChangeMessage();
}

void AudioManager::loadEnvelopes(const File& envelopeFile) {
	if(envelopeFile.exists()) {
		const std::unique_ptr<FileInputStream> stream(envelopeFile.createInputStream());
		envelopeData = std::unique_ptr<float>(new float[stream->getTotalLength() / sizeof(float)]);
		envelopesSize = stream->getTotalLength() / sizeof(float);
		stream->read(&(*envelopeData), stream->getTotalLength());
	}
}

void AudioManager::togglePlayPause() {
	if (isPlaying()) {
		transportSource.stop();
	} else {
		transportSource.start();
	}
}

void AudioManager::performMute() {
	PropertySet& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::AudioMuted);
	const bool muted = !previous;
	properties.setValue(PropertyNames::AudioMuted, muted);

	if (muted) {
		transportSource.setGain(0.0);
	} else {
		transportSource.setGain(1.0);
	}
}

// time is returned as milli beats
int AudioManager::getTime() {
	return roundDoubleToInt(transportSource.getCurrentPosition() * (getBpm() / 60.) * 1000.);
}

// newTime should be in milli beats
void AudioManager::setTime(int newTime) {
	if (newTime == getTime()) {
		return;
	}
	transportSource.setPosition(newTime / (getBpm() / 60.) / 1000.);
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

bool AudioManager::isPlaying() {
	return transportSource.isPlaying();
}

AudioThumbnail& AudioManager::getThumbnail() {
	return thumbnail;
}

int AudioManager::getBpm() {
	return 90;
}

void AudioManager::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case CameraController::playPauseWithAnimation:
		case CameraController::playPauseWithoutAnimation:
			togglePlayPause();
			break;
		case AudioManager::toggleMute:
			performMute();
			break;
	}
}

void AudioManager::applicationCommandListChanged() {
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

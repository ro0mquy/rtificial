#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <juce>

class AudioManager {
	public:
		AudioManager();
		virtual ~AudioManager();

	private:
		AudioFormatManager formatManager;
		AudioDeviceManager deviceManager;
		AudioSourcePlayer player;
		AudioTransportSource transportSource;
		ScopedPointer<AudioFormatReaderSource> currentFileSource;
};

#endif

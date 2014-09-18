#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <juce>

class AudioManager {
	public:
		AudioManager();
		virtual ~AudioManager();

		void loadFile(const File& audioFile);
		void togglePlayPause();

		AudioThumbnail& getThumbnail();
		int getBpm();

	private:
		AudioFormatManager formatManager;
		AudioDeviceManager deviceManager;
		AudioSourcePlayer player;
		AudioTransportSource transportSource;
		ScopedPointer<AudioFormatReaderSource> currentFileSource;
		TimeSliceThread readAheadThread;

		AudioThumbnailCache thumbnailCache;
		AudioThumbnail thumbnail;
};

#endif

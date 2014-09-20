#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <juce>

class AudioManager :
	public ChangeBroadcaster
{
	public:
		AudioManager();
		virtual ~AudioManager();

		static AudioManager& getAudioManager();

		void loadFile(const File& audioFile);
		void togglePlayPause();
		float getTimeInBeats();
		void setTime(float newTimeInBeats);

		AudioThumbnail& getThumbnail();
		int getBpm();

	private:
		class AudioTransportSourceWithCallback :
			public AudioTransportSource
		{
			public:
				AudioTransportSourceWithCallback(AudioManager& audioManager_);
				void getNextAudioBlock(const AudioSourceChannelInfo& info) override;

			private:
				AudioManager& audioManager;
				JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTransportSourceWithCallback)
		};

		AudioFormatManager formatManager;
		AudioDeviceManager deviceManager;
		AudioSourcePlayer player;
		AudioTransportSourceWithCallback transportSource;
		ScopedPointer<AudioFormatReaderSource> currentFileSource;
		TimeSliceThread readAheadThread;

		AudioThumbnailCache thumbnailCache;
		AudioThumbnail thumbnail;
};

#endif

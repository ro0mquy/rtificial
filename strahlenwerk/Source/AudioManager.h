#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <juce>
#include <memory>

class AudioManager :
	public ChangeBroadcaster,
	private ApplicationCommandManagerListener
{
	public:
		AudioManager();
		virtual ~AudioManager();

		static AudioManager& getAudioManager();

		void loadFile(const File& audioFile);
		void loadEnvelopes(const File& envelopeFile);
		void togglePlayPause();
		void performMute();
		int getTime();
		void setTime(int newTimeInBeats);
		float* getCurrentEnvelopes();

		AudioThumbnail& getThumbnail();
		int getBpm();

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) override;
		void applicationCommandListChanged() override;

		enum CommandIDs {
			toggleMute = 0x47e1da00,
		};

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

		std::unique_ptr<float> envelopeData;
		int envelopesSize = 0;
};

#endif

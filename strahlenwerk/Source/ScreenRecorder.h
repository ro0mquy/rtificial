#ifndef SCREEN_RECORDER_H
#define SCREEN_RECORDER_H

#include <juce>

class PostprocPipeline;

class ScreenRecorder :
	private ApplicationCommandManagerListener
{
	public:
		ScreenRecorder(PostprocPipeline& postprocPipeline);

		void applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info);
		void applicationCommandListChanged();

		enum CommandIDs {
			saveScreenshot = 0xdad1337,
		};

	private:
		void performSaveScreenshot();

		class Screenshot :
			private ChangeListener
		{
			public:
				Screenshot(PostprocPipeline& PostprocPipeline);
				~Screenshot();
				void changeListenerCallback(ChangeBroadcaster* /*source*/) override;

				void saveScreenshot();

			private:
				PostprocPipeline& postprocPipeline;

			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Screenshot);
		};

		Screenshot screenshooter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScreenRecorder)
};

#endif

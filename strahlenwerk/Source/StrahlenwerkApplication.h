#ifndef STRAHLENWERKAPPLICATION_H
#define STRAHLENWERKAPPLICATION_H

#include <juce>
#include "Project/Project.h"

class MainWindow;
class AudioManager;
class RtificialLookAndFeel;

class StrahlenwerkApplication : public JUCEApplication {
	public:
		~StrahlenwerkApplication();

		const String getApplicationName() override;
		const String getApplicationVersion() override;
		bool moreThanOneInstanceAllowed() override;
		void initialise(const String& commandLine) override;
		void shutdown() override;
		void systemRequestedQuit() override;
		void anotherInstanceStarted(const String& commandLine) override;

		MainWindow& getMainWindow();
		Project& getProject();
		PropertySet& getProperties();
		AudioManager& getAudioManager();

		static StrahlenwerkApplication* getInstance();

	private:
		ScopedPointer<RtificialLookAndFeel> lookAndFeel;
		ScopedPointer<MainWindow> mainWindow;
		ScopedPointer<AudioManager> audioManager;
		ScopedPointer<Project> project;
		ScopedPointer<PropertiesFile> properties;
};

#endif

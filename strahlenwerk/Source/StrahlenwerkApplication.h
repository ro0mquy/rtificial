#ifndef STRAHLENWERKAPPLICATION_H
#define STRAHLENWERKAPPLICATION_H

#include <juce>
#include "Project.h"

class MainWindow;

class StrahlenwerkApplication : public JUCEApplication {
	public:
		const String getApplicationName() override;
		const String getApplicationVersion() override;
		bool moreThanOneInstanceAllowed() override;
		void initialise(const String& commandLine) override;
		void shutdown() override;
		void systemRequestedQuit() override;
		void anotherInstanceStarted(const String& commandLine) override;

		Project& getProject();

		static StrahlenwerkApplication* getInstance();

	private:
		ScopedPointer<MainWindow> mainWindow;
		Project project;
};

#endif

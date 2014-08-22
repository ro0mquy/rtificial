#ifndef STRAHLENWERKAPPLICATION_H
#define STRAHLENWERKAPPLICATION_H

#include <juce>

class MainWindow;
class Project;

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

		Project& getProject();
		PropertySet& getProperties();

		static StrahlenwerkApplication* getInstance();

	private:
		ScopedPointer<MainWindow> mainWindow;
		ScopedPointer<Project> project;
		ScopedPointer<PropertiesFile> properties;
};

#endif

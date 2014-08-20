#ifndef STRAHLENWERKAPPLICATION_H
#define STRAHLENWERKAPPLICATION_H

#include <juce>

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

	private:
		ScopedPointer<MainWindow> mainWindow;
};

#endif

#include <juce>
#include "MainWindow.h"

class strahlenwerkApplication : public JUCEApplication {
	public:
		strahlenwerkApplication() {}

		const String getApplicationName() override	   { return ProjectInfo::projectName; }
		const String getApplicationVersion() override	{ return ProjectInfo::versionString; }
		bool moreThanOneInstanceAllowed() override	   { return true; }

		void initialise(const String& commandLine) override {
			mainWindow = new MainWindow();
		}

		void shutdown() override {
			// Add your application's shutdown code here..
			mainWindow = nullptr; // (deletes our window)
		}

		void systemRequestedQuit() override {
			// This is called when the app is being asked to quit: you can ignore this
			// request and let the app carry on running, or call quit() to allow the app to close.
			MainWindow::getApplicationCommandManager().invokeDirectly(MainWindow::quitProgram, false);
		}

		void anotherInstanceStarted(const String& commandLine) override {
			// When another instance of the app is launched while this one is running,
			// this method is invoked, and the commandLine parameter tells you what
			// the other instance's command-line arguments were.
		}

	private:
		ScopedPointer<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(strahlenwerkApplication)

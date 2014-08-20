#include "StrahlenwerkApplication.h"

#include "MainWindow.h"

const String StrahlenwerkApplication::getApplicationName() {
	return ProjectInfo::projectName;
}

const String StrahlenwerkApplication::getApplicationVersion() {
	return ProjectInfo::versionString;
}

bool StrahlenwerkApplication::moreThanOneInstanceAllowed() {
	return true;
}

void StrahlenwerkApplication::initialise(const String& commandLine) {
	mainWindow = new MainWindow();
}

void StrahlenwerkApplication::shutdown() {
	// Add your application's shutdown code here..
	mainWindow = nullptr; // (deletes our window)
}

void StrahlenwerkApplication::systemRequestedQuit()  {
	// This is called when the app is being asked to quit: you can ignore this
	// request and let the app carry on running, or call quit() to allow the app to close.
	MainWindow::getApplicationCommandManager().invokeDirectly(MainWindow::quitProgram, false);
}

void StrahlenwerkApplication::anotherInstanceStarted(const String& commandLine) {
	// When another instance of the app is launched while this one is running,
	// this method is invoked, and the commandLine parameter tells you what
	// the other instance's command-line arguments were.
}

START_JUCE_APPLICATION(StrahlenwerkApplication)

#include "StrahlenwerkApplication.h"

#include "MainWindow.h"
#include "Rendering/PostprocShader.h"
#include "Rendering/PostprocPipeline.h"
#include "Project.h"
#include "PropertyNames.h"

StrahlenwerkApplication::~StrahlenwerkApplication() = default;

const String StrahlenwerkApplication::getApplicationName() {
	return ProjectInfo::projectName;
}

const String StrahlenwerkApplication::getApplicationVersion() {
	return ProjectInfo::versionString;
}

bool StrahlenwerkApplication::moreThanOneInstanceAllowed() {
	return false;
}

void StrahlenwerkApplication::initialise(const String& commandLine) {
	PropertiesFile::Options options;
	options.applicationName = getApplicationName();
	options.filenameSuffix = ".settings";
	options.osxLibrarySubFolder = "Application Support";
	properties = new PropertiesFile(options);

	project = new Project(properties->getValue(PropertyNames::PROJECT_DIR, "./").toStdString());
	mainWindow = new MainWindow();
}

void StrahlenwerkApplication::shutdown() {
	// Add your application's shutdown code here..
	mainWindow = nullptr; // (deletes our window)
	project = nullptr;
	properties = nullptr;
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

Project& StrahlenwerkApplication::getProject() {
	return *project;
}

PropertySet& StrahlenwerkApplication::getProperties() {
	return *properties;
}

StrahlenwerkApplication* StrahlenwerkApplication::getInstance() {
	return static_cast<StrahlenwerkApplication*>(JUCEApplication::getInstance());
}

START_JUCE_APPLICATION(StrahlenwerkApplication)

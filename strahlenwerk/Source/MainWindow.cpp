#include "MainWindow.h"

#include "StrahlenwerkApplication.h"
#include <Renderer.h>
#include <Timeline/SpecialUniformController.h>
#include <OpenGLComponent.h>
#include <Project/Project.h>

// global ApplicationCommandManager, stores all commands and provides shortcuts
// access via MainWindow::getApplicationCommandManager()
static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

/*
   This class implements the desktop window that contains an instance of
   our MainContentComponent class.
   */
MainWindow::MainWindow() :
	DocumentWindow (ProjectInfo::projectName,
			Colours::darkgrey,
			DocumentWindow::allButtons)
{
	setName("strahlenwerk - BER edition");
	setMenuBar(this);
	setContentOwned(&mainContentComponent, true);

	centreWithSize(getWidth(), getHeight());
	setVisible(true);
	setUsingNativeTitleBar(true);
	setResizable(true, true);

	// let commandManager handle commands and key presses
	getApplicationCommandManager().registerAllCommandsForTarget(this);
	addKeyListener(getApplicationCommandManager().getKeyMappings());
	setApplicationCommandManagerToWatch(&getApplicationCommandManager());
}

MainWindow::~MainWindow() {
	applicationCommandManager = nullptr;
	setMenuBar(nullptr);
}

void MainWindow::closeButtonPressed() {
	// This is called when the user tries to close this window. Here, we'll just
	// ask the app to quit when this happens, but you can change this to do
	// whatever you need.
	getApplicationCommandManager().invokeDirectly(MainWindow::quitProgram, false);
}

MainContentComponent& MainWindow::getMainContentComponent() {
	return mainContentComponent;
}

ApplicationCommandManager& MainWindow::getApplicationCommandManager() {
	if (applicationCommandManager == nullptr) {
		applicationCommandManager = new ApplicationCommandManager();
	}

	return *applicationCommandManager;
}

// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.

// For adding new commands add them to getAllCommands() and getCommandInfo()
// and handle them via callback in the class that performs the action
// may also add them to the menu bar model below

ApplicationCommandTarget* MainWindow::getNextCommandTarget() {
	// this will return the next parent component that is an ApplicationCommandTarget
	return findFirstTargetParentComponent();
}

void MainWindow::getAllCommands(Array<CommandID>& commands) {
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = {
		MainWindow::quitProgram,
		MainWindow::toggleFullscreen,
		Project::openProject,
		Project::reloadShaderFiles,
		Project::saveTimeline,
		OpenGLComponent::toggleGrid,
		CameraController::playPauseWithAnimation,
		CameraController::playPauseWithoutAnimation,
		CameraController::setKeyframe,
		CameraController::resetCameraPosition,
		CameraController::resetCameraRotation,
		Renderer::makeDemo,

	};

	commands.addArray(ids, numElementsInArray(ids));
}

void MainWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) {
	const String programCategory("Program");

	// supply info: shortname, description, category, flags(=0)
	switch (commandID) {

		case MainWindow::quitProgram:
			result.setInfo("Quit!", "Shuts down all the Beam Factory!", programCategory, 0);
			result.addDefaultKeypress('q', ModifierKeys::commandModifier);
			break;

		case MainWindow::toggleFullscreen:
			result.setInfo("Toogle Fullscreen", "Toggle fullscreen mode for OpenGL component", programCategory, 0);
			result.addDefaultKeypress('f', ModifierKeys::noModifiers);
			break;

		case Project::openProject:
			result.setInfo("Open Project", "Open a strahlenwerk project", programCategory, 0);
			result.addDefaultKeypress('o', ModifierKeys::commandModifier);
			break;

		case Project::reloadShaderFiles:
			result.setInfo("Reload Shaders", "Reload all Shaders", programCategory, 0);
			result.addDefaultKeypress('r', ModifierKeys::commandModifier);
			break;

		case Project::saveTimeline:
			result.setInfo("Save Timeline", "Save the timeline data to a file", programCategory, 0);
			result.addDefaultKeypress('s', ModifierKeys::commandModifier);
			break;

		case CameraController::playPauseWithAnimation:
			result.setInfo("Play/Pause w/ animation", "Toggle play/pause with animation of camera", programCategory, 0);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::shiftModifier);
			break;

		case CameraController::playPauseWithoutAnimation:
			result.setInfo("Play/Pause w/o animation", "Toggle play/pause without animation of camera", programCategory, 0);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::noModifiers);
			break;

		case OpenGLComponent::toggleGrid:
			result.setInfo("Toogle Grid", "Enable/Disable Rule of Thirds grid overlay", programCategory, 0);
			result.addDefaultKeypress('g', ModifierKeys::noModifiers);
			break;

		case CameraController::setKeyframe:
			result.setInfo("Set Keyframe", "Places a Keyframe for the Camera at the Current Position", programCategory, 0);
			result.addDefaultKeypress('p', ModifierKeys::noModifiers);
			break;

		case CameraController::resetCameraPosition:
			result.setInfo("Reset Camera Position", "Moves the Camera back to the Origin", programCategory, 0);
			result.addDefaultKeypress('b', ModifierKeys::commandModifier);
			break;

		case CameraController::resetCameraRotation:
			result.setInfo("Reset Camera Rotation", "Sets the Camera Rotation back to Default", programCategory, 0);
			result.addDefaultKeypress('b', ModifierKeys::noModifiers);
			break;

		case Renderer::makeDemo:
			result.setInfo("Make demo", "Make a demo about it!", programCategory, 0);
			result.addDefaultKeypress('d', ModifierKeys::commandModifier);
			break;

		default:
			break;

	}
}

bool MainWindow::perform(const InvocationInfo& info) {
	// do something to perform the command
	// most of the commands get handled somewhere else via callback
	switch (info.commandID) {
		case MainWindow::quitProgram:
			JUCEApplication::getInstance()->quit();
			break;

		case MainWindow::toggleFullscreen:
			performToggleFullscreen();
			break;
	}

	return true;
}

// Following functions implements the MenuBarModel interface
// it provides the content for a menu bar

StringArray MainWindow::getMenuBarNames() {
	const char* const names[] = { "File", "Build", nullptr };
	return StringArray(names);
}

// build menus for top level indexes
PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const String& /*menuName*/) {
	ApplicationCommandManager* commandManager = &getApplicationCommandManager();
	PopupMenu menu;

	if (topLevelMenuIndex == 0) {
		menu.addCommandItem(commandManager, Project::openProject);
		menu.addCommandItem(commandManager, Project::saveTimeline);
		menu.addCommandItem(commandManager, Project::reloadShaderFiles);
		menu.addCommandItem(commandManager, OpenGLComponent::toggleGrid);
		menu.addCommandItem(commandManager, MainWindow::toggleFullscreen);
		menu.addCommandItem(commandManager, CameraController::setKeyframe);
		menu.addCommandItem(commandManager, CameraController::resetCameraPosition);
		menu.addCommandItem(commandManager, CameraController::resetCameraRotation);
		menu.addCommandItem(commandManager, MainWindow::quitProgram);
	} else if (topLevelMenuIndex == 1) {
		menu.addCommandItem(commandManager, Renderer::makeDemo);
	}

	return menu;
}

// react on menu items that are not handled by the commandManager
void MainWindow::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) {
	if (menuItemID == 23) {
		// dummy
	}
}

void MainWindow::performToggleFullscreen() {
	auto& desktop = Desktop::getInstance();
	if(this == desktop.getKioskModeComponent()) {
		mainContentComponent.setDefaultLayout();
		desktop.setKioskModeComponent(nullptr);
		// doppelt hält besser
		setFullScreen(false);
	} else {
		mainContentComponent.setOpenGLOnlyLayout();
		desktop.setKioskModeComponent(this);
		setFullScreen(true);
	}
}

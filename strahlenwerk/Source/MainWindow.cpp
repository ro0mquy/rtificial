#include "MainWindow.h"

#include "StrahlenwerkApplication.h"
#include "Project/Project.h"
#include "AudioManager.h"
#include "PropertyNames.h"

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

ApplicationCommandTarget* MainWindow::getNextCommandTarget() {
	// this will return the next parent component that is an ApplicationCommandTarget
	return findFirstTargetParentComponent();
}

void MainWindow::getAllCommands(Array<CommandID>& commands) {
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = {
		MainWindow::quitProgram,
		MainWindow::openProject,
		MainWindow::reload,
		MainWindow::saveTimeline,
		MainWindow::playPauseWithAnimation,
		MainWindow::playPauseWithoutAnimation,
		MainWindow::toggleGrid,
		MainWindow::toggleFullscreen,
		MainWindow::setKeyframe,
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
		case MainWindow::openProject:
			result.setInfo("Open Project", "Open a strahlenwerk project", programCategory, 0);
			result.addDefaultKeypress('o', ModifierKeys::commandModifier);
			break;
		case MainWindow::reload:
			result.setInfo("Reload Shaders", "Reload all Shaders", programCategory, 0);
			result.addDefaultKeypress('r', ModifierKeys::commandModifier);
			break;
		case MainWindow::saveTimeline:
			result.setInfo("Save Timeline", "Save the timeline data to a file", programCategory, 0);
			result.addDefaultKeypress('s', ModifierKeys::commandModifier);
			break;
		case MainWindow::playPauseWithAnimation:
			result.setInfo("Play/Pause w/ animation", "Toggle play/pause with animation of camera", programCategory, 0);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::shiftModifier);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::commandModifier); // TODO: remove this
			break;
		case MainWindow::playPauseWithoutAnimation:
			result.setInfo("Play/Pause w/o animation", "Toggle play/pause without animation of camera", programCategory, 0);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::noModifiers);
			break;
		case MainWindow::toggleGrid:
			result.setInfo("Toogle Grid", "Enable/Disable Rule of Thirds grid overlay", programCategory, 0);
			result.addDefaultKeypress('g', ModifierKeys::noModifiers);
			break;
		case MainWindow::toggleFullscreen:
			result.setInfo("Toogle Fullscreen", "Toggle fullscreen mode for OpenGL component", programCategory, 0);
			result.addDefaultKeypress('f', ModifierKeys::noModifiers);
			break;
		case MainWindow::setKeyframe:
			result.setInfo("Set Keyframe", "Places a Keyframe for the Camera at the Current Position", programCategory, 0);
			result.addDefaultKeypress('p', ModifierKeys::noModifiers);
		default:
			break;
	}
}

bool MainWindow::perform(const InvocationInfo& info) {
	// do something to perform the command
	switch (info.commandID) {
		case MainWindow::quitProgram:
			JUCEApplication::getInstance()->quit();
			break;

		case MainWindow::openProject:
			doOpenProject();
			break;

		case MainWindow::reload:
			doReload();
			break;

		case MainWindow::saveTimeline:
			doSaveTimeline();
			break;

		case MainWindow::playPauseWithAnimation:
		case MainWindow::playPauseWithoutAnimation:
			// control switching gets done via command listener
			doPlayPause();
			break;

		case MainWindow::toggleGrid:
			doToggleGrid();
			break;

		case MainWindow::toggleFullscreen:
			doToggleFullscreen();
			break;

		case MainWindow::setKeyframe:
			// do nothing here, the action is done by a command listener
			break;

		default:
			return false;
	}

	return true;
}

// Following functions implements the MenuBarModel interface
// it provides the content for a menu bar

StringArray MainWindow::getMenuBarNames() {
	const char* const names[] = { "File", nullptr };
	return StringArray(names);
}

// build menus for top level indexes
PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const String& /*menuName*/) {
	ApplicationCommandManager* commandManager = &getApplicationCommandManager();
	PopupMenu menu;

	if (topLevelMenuIndex == 0) {
		menu.addCommandItem(commandManager, MainWindow::openProject);
		menu.addCommandItem(commandManager, MainWindow::saveTimeline);
		menu.addCommandItem(commandManager, MainWindow::reload);
		menu.addCommandItem(commandManager, MainWindow::toggleGrid);
		menu.addCommandItem(commandManager, MainWindow::toggleFullscreen);
		menu.addCommandItem(commandManager, MainWindow::setKeyframe);
		menu.addCommandItem(commandManager, MainWindow::quitProgram);
	}

	return menu;
}

// react on menu items that are not handled by the commandManager
void MainWindow::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) {
	if (menuItemID == 23) {
		// dummy
	}
}

void MainWindow::doOpenProject() {
	FileChooser fileChooser("Entscheide dich gefaelligst!");
	if(fileChooser.browseForDirectory()) {
		auto path = fileChooser.getResult().getFullPathName().toStdString();
		StrahlenwerkApplication::getInstance()->getProject().loadDirectory(path);
	}
}

void MainWindow::doReload() {
	StrahlenwerkApplication::getInstance()->getProject().reloadShaders();
}

void MainWindow::doSaveTimeline() {
	StrahlenwerkApplication::getInstance()->getProject().saveTimelineData();
}

void MainWindow::doPlayPause() {
	AudioManager::getAudioManager().togglePlayPause();
}

void MainWindow::doToggleGrid() {
	auto& properties = StrahlenwerkApplication::getInstance()->getProperties();
	const bool previous = properties.getBoolValue(PropertyNames::GRID_ENABLED);
	properties.setValue(PropertyNames::GRID_ENABLED, !previous);
	// TODO this is not very elegant
	mainContentComponent.repaintOpenGLComponent();
}

void MainWindow::doToggleFullscreen() {
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

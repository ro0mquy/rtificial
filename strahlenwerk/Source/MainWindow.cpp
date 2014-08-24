#include "MainWindow.h"

#include "StrahlenwerkApplication.h"
#include "Project/Project.h"

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
	setLookAndFeel(&lookAndFeel);
	setMenuBar(this);
	setContentOwned(&mainComponent, true);

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
	};

	commands.addArray(ids, numElementsInArray(ids));
}

void MainWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) {
	const String programCategory("Program");

	// supply info: shortname, description, category, flags(=0)
	switch (commandID) {
		case MainWindow::quitProgram:
			result.setInfo("Quit!", "Shuts down all the Beam Factory!", programCategory, 0);
			result.addDefaultKeypress(KeyPress::escapeKey, ModifierKeys::noModifiers);
			break;
		case MainWindow::openProject:
			result.setInfo("Open Project", "Open a strahlenwerk project", programCategory, 0);
			break;
		case MainWindow::reload:
			result.setInfo("Reload", "Reload everything", programCategory, 0);
			result.addDefaultKeypress('r', ModifierKeys::noModifiers);
			break;
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
PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const String& menuName) {
	ApplicationCommandManager* commandManager = &getApplicationCommandManager();
	PopupMenu menu;

	if (topLevelMenuIndex == 0) {
		menu.addCommandItem(commandManager, MainWindow::openProject);
		menu.addCommandItem(commandManager, MainWindow::reload);
		menu.addCommandItem(commandManager, MainWindow::quitProgram);
	}

	return menu;
}

// react on menu items that are not handled by the commandManager
void MainWindow::menuItemSelected(int menuItemID, int topLevelMenuIndex) {
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
	StrahlenwerkApplication::getInstance()->getProject().reload();
}

#include "MainWindow.h"

#include "StrahlenwerkApplication.h"
#include <Renderer.h>
#include <Timeline/CameraController.h>
#include <OpenGLComponent.h>
#include <Project/Project.h>
#include <Timeline/TimelineData.h>
#include <AudioManager.h>
#include <ScreenRecorder.h>

// global ApplicationCommandManager, stores all commands and provides shortcuts
// access via MainWindow::getApplicationCommandManager()
static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

/*
   This class implements the desktop window that contains an instance of
   our MainContentComponent class.
   */
MainWindow::MainWindow() :
	DocumentWindow (ProjectInfo::projectName,
			LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), // nicht mehr kritzelila
			DocumentWindow::allButtons)
{
	setWindowTitle();
	setMenuBar(this);
	setContentOwned(&mainContentComponent, true);

	centreWithSize(getWidth(), getHeight());
	setVisible(true);
	setUsingNativeTitleBar(true);
	setResizable(true, true);

	// let commandManager handle commands and key presses
	ApplicationCommandManager& appComManager = getApplicationCommandManager();
	appComManager.registerAllCommandsForTarget(this);
	appComManager.setFirstCommandTarget(this);
	addKeyListener(appComManager.getKeyMappings());
	setApplicationCommandManagerToWatch(&appComManager);

	TimelineData::getTimelineData().getUndoManager().addChangeListener(this);
	StrahlenwerkApplication::getInstance()->getProject().addListener(this);
}

MainWindow::~MainWindow() {
	applicationCommandManager = nullptr;
	setMenuBar(nullptr);
	StrahlenwerkApplication::getInstance()->getProject().removeListener(this);
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
		Project::reloadTimeline,
		Project::reloadEnvironments,
		OpenGLComponent::toggleGrid,
		Renderer::toggleHalfResolution,
		CameraController::playPauseWithAnimation,
		CameraController::playPauseWithoutAnimation,
		CameraController::setKeyframe,
		CameraController::resetCameraPosition,
		CameraController::resetCameraRotation,
		Renderer::makeDemo,
		TimelineData::undoAction,
		TimelineData::redoAction,
		TimelineData::bakeOver,
		AudioManager::toggleMute,
		Selection::toggleLoop,
		Selection::deleteSelection,
		Selection::splitSequence,
		Selection::mergeSequence,
		CameraController::increaseMovementSpeed,
		CameraController::decreaseMovementSpeed,
		CameraController::increaseRotationSpeed,
		CameraController::decreaseRotationSpeed,
		ScreenRecorder::saveScreenshot,
	};

	commands.addArray(ids, numElementsInArray(ids));
}

void MainWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) {
	const String programCategory("Program");

	// supply info: shortname, description, category, flags(=0)
	switch (commandID) {

		case MainWindow::quitProgram:
			result.setInfo("Quit", "Shuts down all the strahlenwerk!", programCategory, 0);
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
			break;

		case Project::saveTimeline:
			result.setInfo("Save Timeline", "Save the timeline data to a file", programCategory, 0);
			result.addDefaultKeypress('s', ModifierKeys::commandModifier);
			break;

		case Project::reloadTimeline:
			result.setInfo("Reload Timeline", "Reload Timeline data from file and discard local changes", programCategory, 0);
			result.addDefaultKeypress('r', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
			break;

		case Project::reloadEnvironments:
			result.setInfo("Reload Environments", "Reload Environment Maps shaders and re-render", programCategory, 0);
			result.addDefaultKeypress('r', ModifierKeys::commandModifier);
			break;

		case CameraController::playPauseWithAnimation:
			result.setInfo("Play/Pause w/ Animation", "Toggle play/pause with animation of camera", programCategory, 0);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::shiftModifier);
			break;

		case CameraController::playPauseWithoutAnimation:
			result.setInfo("Play/Pause w/o Animation", "Toggle play/pause without animation of camera", programCategory, 0);
			result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::noModifiers);
			break;

		case OpenGLComponent::toggleGrid:
			result.setInfo("Toogle Grid", "Enable/Disable Rule of Thirds grid overlay", programCategory, 0);
			result.addDefaultKeypress('g', ModifierKeys::noModifiers);
			break;

		case Renderer::toggleHalfResolution:
			result.setInfo("Toogle Half Resolution", "Enable/Disable rendering at half resolution", programCategory, 0);
			result.addDefaultKeypress('h', ModifierKeys::commandModifier);
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
			result.setInfo("Make Demo", "Make a demo about it!", programCategory, 0);
			result.addDefaultKeypress('d', ModifierKeys::commandModifier);
			break;

		case AudioManager::toggleMute:
			result.setInfo("Toggle Audio Mute", "Mutes or unmutes the audio track", programCategory, 0);
			result.addDefaultKeypress('m', ModifierKeys::commandModifier);
			break;

		case TimelineData::undoAction:
			{
				const UndoManager& undoMgr = TimelineData::getTimelineData().getUndoManager();
				const bool isActive = undoMgr.canUndo();
				const String actionName = "Undo " + undoMgr.getUndoDescription();

				result.setInfo(actionName, actionName, programCategory, 0);
				result.setActive(isActive);
				result.addDefaultKeypress('z', ModifierKeys::commandModifier);
			}
			break;

		case TimelineData::redoAction:
			{
				const UndoManager& undoMgr = TimelineData::getTimelineData().getUndoManager();
				const bool isActive = undoMgr.canRedo();
				const String actionName = "Redo " + undoMgr.getRedoDescription();

				result.setInfo(actionName, actionName, programCategory, 0);
				result.setActive(isActive);
				result.addDefaultKeypress('z', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
			}
			break;

		case TimelineData::bakeOver:
			result.setInfo("Bake Over", "Bake all unanimated uniforms", programCategory, 0);
			break;

		case Selection::toggleLoop:
			result.setInfo("Toggle Selection Looping", "Loops the current selection", programCategory, 0);
			result.addDefaultKeypress('8', ModifierKeys::noModifiers);
			break;

		case Selection::deleteSelection:
			result.setInfo("Delete Selection", "Deletes the current selection", programCategory, 0);
			result.addDefaultKeypress(KeyPress::deleteKey, ModifierKeys::noModifiers);
			result.addDefaultKeypress(KeyPress::backspaceKey, ModifierKeys::noModifiers);
			result.addDefaultKeypress('`', ModifierKeys::noModifiers);
			break;

		case Selection::splitSequence:
			result.setInfo("Split Sequence", "Splits the current selected sequence", programCategory, 0);
			result.addDefaultKeypress('t', ModifierKeys::noModifiers);
			break;

		case Selection::mergeSequence:
			result.setInfo("Merge Sequences", "Merges selected sequences", programCategory, 0);
			result.addDefaultKeypress('t', ModifierKeys::commandModifier);
			break;

		case CameraController::increaseMovementSpeed:
			result.setInfo("Increase Movement Speed", "Increases the movement speed of the camera", programCategory, 0);
			result.addDefaultKeypress('=', ModifierKeys::noModifiers);
			break;

		case CameraController::decreaseMovementSpeed:
			result.setInfo("Decrease Movement Speed", "Decreases the movement speed of the camera", programCategory, 0);
			result.addDefaultKeypress('-', ModifierKeys::noModifiers);
			break;

		case CameraController::increaseRotationSpeed:
			result.setInfo("Increase Rotation Speed", "Increases the rotation speed of the camera", programCategory, 0);
			result.addDefaultKeypress('=', ModifierKeys::commandModifier);
			break;

		case CameraController::decreaseRotationSpeed:
			result.setInfo("Decrease Rotation Speed", "Decreases the rotation speed of the camera", programCategory, 0);
			result.addDefaultKeypress('-', ModifierKeys::commandModifier);
			break;

		case ScreenRecorder::saveScreenshot:
			result.setInfo("Save Screenshot", "Save current view to disk as PNG", programCategory, 0);
			result.addDefaultKeypress(KeyPress::F12Key, ModifierKeys::noModifiers);
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
	const char* const names[] = { "File", "Edit", "View", "Timeline", "Help", nullptr };
	return StringArray(names);
}

// build menus for top level indexes
PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const String& /*menuName*/) {
	ApplicationCommandManager* commandManager = &getApplicationCommandManager();
	PopupMenu menu;

	if (topLevelMenuIndex == 0 /* File */) {
		menu.addCommandItem(commandManager, Project::openProject);
		menu.addCommandItem(commandManager, Project::reloadShaderFiles);
		menu.addCommandItem(commandManager, Project::reloadTimeline);
		menu.addCommandItem(commandManager, Project::reloadEnvironments);
		menu.addSeparator();
		menu.addCommandItem(commandManager, Project::saveTimeline);
		menu.addSeparator();
		menu.addCommandItem(commandManager, ScreenRecorder::saveScreenshot);
		menu.addSeparator();
		menu.addCommandItem(commandManager, Renderer::makeDemo);
		menu.addSeparator();
		menu.addCommandItem(commandManager, MainWindow::quitProgram);
	} else if (topLevelMenuIndex == 1 /* Edit */) {
		menu.addCommandItem(commandManager, TimelineData::undoAction);
		menu.addCommandItem(commandManager, TimelineData::redoAction);
	} else if (topLevelMenuIndex == 2 /* View */) {
		menu.addCommandItem(commandManager, OpenGLComponent::toggleGrid);
		menu.addCommandItem(commandManager, Renderer::toggleHalfResolution);
		menu.addCommandItem(commandManager, MainWindow::toggleFullscreen);
	} else if (topLevelMenuIndex == 3 /* Timeline */) {
		menu.addCommandItem(commandManager, CameraController::setKeyframe);
		menu.addCommandItem(commandManager, Selection::splitSequence);
		menu.addCommandItem(commandManager, Selection::mergeSequence);
		menu.addSeparator();
		menu.addCommandItem(commandManager, CameraController::resetCameraPosition);
		menu.addCommandItem(commandManager, CameraController::resetCameraRotation);
		menu.addCommandItem(commandManager, CameraController::increaseMovementSpeed);
		menu.addCommandItem(commandManager, CameraController::decreaseMovementSpeed);
		menu.addCommandItem(commandManager, CameraController::increaseRotationSpeed);
		menu.addCommandItem(commandManager, CameraController::decreaseRotationSpeed);
		menu.addSeparator();
		menu.addCommandItem(commandManager, AudioManager::toggleMute);
		menu.addSeparator();
		menu.addCommandItem(commandManager, Selection::toggleLoop);
		menu.addSeparator();
		menu.addCommandItem(commandManager, TimelineData::bakeOver);
	} else if (topLevelMenuIndex == 4 /* Help */) {
		menu.addItem(41, "No Help available. You are lost.", false);
		menu.addSeparator();
		menu.addItem(42, "About");
	}

	return menu;
}

// react on menu items that are not handled by the commandManager
void MainWindow::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) {
	if (menuItemID == 42) {
		AlertWindow aboutWindow("About", "Strahlenwerk is brought to you by rtificial.", AlertWindow::AlertIconType::InfoIcon);
		aboutWindow.addButton("Close", 0, KeyPress('c'), KeyPress(KeyPress::escapeKey));
		aboutWindow.runModalLoop();
	}
}

void MainWindow::setWindowTitle() {
	StrahlenwerkApplication* strahlenwerkApp = StrahlenwerkApplication::getInstance();
	setName(strahlenwerkApp->getProject().getProjectName()
		+ " - "
		+ strahlenwerkApp->getApplicationName()
		+ " ("
		+ "Elbphilharmonie" //StrahlenwerkApplication::getInstance()->getApplicationVersion()
		+ " Edition)"
	);
}

void MainWindow::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	// undoManager changed
	getApplicationCommandManager().commandStatusChanged();
}

void MainWindow::projectChanged() {
	setWindowTitle();
}

void MainWindow::performToggleFullscreen() {
	auto& desktop = Desktop::getInstance();
	if(this == desktop.getKioskModeComponent()) {
		mainContentComponent.setDefaultLayout();
		desktop.setKioskModeComponent(nullptr);
		// doppelt hält besser
		setFullScreen(false);
	} else {
		// first set kiosk mode, so the OpenGL component knows it
		// doesn't need to write it's new size to the settings file
		desktop.setKioskModeComponent(this);
		mainContentComponent.setOpenGLOnlyLayout();
		setFullScreen(true);
	}
}

void MainWindow::modifierKeysChanged(const ModifierKeys &modifiers) {
	// also call keyListeners when modifiers change
	getPeer()->handleKeyUpOrDown(false);
	Component::modifierKeysChanged(modifiers);
}

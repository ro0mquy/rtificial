#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <juce>
#include "MainContentComponent.h"

/*
   This class implements the desktop window that contains an instance of
   our MainContentComponent class.
   */
class MainWindow : public DocumentWindow,
	public ApplicationCommandTarget,
	public MenuBarModel
{
	public:
		MainWindow();
		~MainWindow();
		void closeButtonPressed();

		static ApplicationCommandManager& getApplicationCommandManager();
		MainContentComponent& getMainContentComponent();

		enum CommandIDs {
			quitProgram = 0x2300,
			openProject,
			reload,
			saveTimeline,
			playPause,
			toggleGrid,
			toggleFullscreen,
			setKeyframe,
		};

		StringArray getMenuBarNames() override;
		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

	private:
		MainContentComponent mainContentComponent;

		ApplicationCommandTarget* getNextCommandTarget() override;
		void getAllCommands(Array<CommandID>& commands) override;
		void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
		bool perform(const InvocationInfo& info) override;
		void doOpenProject();
		void doReload();
		void doSaveTimeline();
		void doPlayPause();
		void doToggleGrid();
		void doToggleFullscreen();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

#endif  // MAINWINDOW_H

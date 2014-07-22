#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

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

		enum CommandIDs {
			quitProgram = 0x2300,
		};

		StringArray getMenuBarNames() override;
		PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
		void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

	private:
		MainContentComponent mainComponent;
		LookAndFeel_V3 lookAndFeel;

		ApplicationCommandTarget* getNextCommandTarget() override;
		void getAllCommands(Array<CommandID>& commands) override;
		void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
		bool perform(const InvocationInfo& info) override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

#endif  // MAINWINDOW_H

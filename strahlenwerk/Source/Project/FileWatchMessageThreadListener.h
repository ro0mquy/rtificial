#ifndef FILEWATCHMESSAGETHREADLISTENER_H
#define FILEWATCHMESSAGETHREADLISTENER_H

#include <efsw/efsw.hpp>
#include <string>
#include <juce>

class FileWatchMessageThreadListener :
	public efsw::FileWatchListener,
	private MessageListener
{
	public:
		FileWatchMessageThreadListener(FileWatchListener& listener);
		void handleFileAction(
				efsw::WatchID watchid,
				const std::string& dir,
				const std::string& filename,
				efsw::Action action,
				std::string oldFilename = "") override;

	private:
		void handleMessage(const Message& message) override;

		efsw::FileWatchListener& listener;
};

#endif

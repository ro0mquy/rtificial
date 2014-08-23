#include "FileWatchMessageThreadListener.h"

struct FileActionMessage : public Message {
	FileActionMessage(
		efsw::WatchID watchid,
		std::string dir,
		std::string filename,
		efsw::Action action,
		std::string oldFilename
	) :
		watchid(watchid),
		dir(dir),
		filename(filename),
		action(action),
		oldFilename(oldFilename)
	{}

	efsw::WatchID watchid;
	std::string dir;
	std::string filename;
	efsw::Action action;
	std::string oldFilename;
};

FileWatchMessageThreadListener::FileWatchMessageThreadListener(FileWatchListener& listener) :
	listener(listener)
{
}

void FileWatchMessageThreadListener::handleFileAction(
		efsw::WatchID watchid,
		const std::string& dir,
		const std::string& filename,
		efsw::Action action,
		std::string oldFilename)
{
	postMessage(new FileActionMessage(watchid, dir, filename, action, oldFilename));
}

void FileWatchMessageThreadListener::handleMessage(const Message& message) {
	auto& args = dynamic_cast<const FileActionMessage&>(message);
	listener.handleFileAction(args.watchid, args.dir, args.filename, args.action, args.oldFilename);
}

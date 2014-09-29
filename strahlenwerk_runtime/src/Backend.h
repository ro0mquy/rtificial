#ifndef BACKEND_H
#define BACKEND_H

using int16_t = short;

// interface for backends
class EmptyBackend {
	void init(int width, int height, bool fullscreen);
	void play_music(int16_t* audio);
	// return false for abort
	bool beforeFrame();
	void afterFrame();
	void cleanup();
};

class WindowsBackend {
	public:
		void init(int width, int height, bool fullscreen);
		void play_music(int16_t* audio);
		bool beforeFrame();
		void afterFrame();
		void cleanup();
};

class LinuxBackend {
	public:
		void init(int width, int height, bool fullscreen);
		void play_music(int16_t* audio);
		bool beforeFrame();
		void afterFrame();
		void cleanup();
};

#endif

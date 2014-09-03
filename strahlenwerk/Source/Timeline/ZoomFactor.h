#ifndef ZOOMFACTOR_H
#define ZOOMFACTOR_H

#include <juce>
#include <mutex>

class ZoomFactor {
	public:
		ZoomFactor();

		static ZoomFactor& getZoomFactor();

		operator float();
		ZoomFactor& operator=(const float newZoomLevel);
		ZoomFactor& operator*=(const float zoomLevelFactor);
		ZoomFactor& operator/=(const float zoomLevelDivisor);

		float timeToPixels(const float time);
		float pixelsToTime(const float pixels);

		class Listener {
			public:
				virtual void zoomFactorChanged(ZoomFactor& zoomFactorThatHasChanged) = 0;
		};

		void addListener(Listener* const listener);
		void removeListener(Listener* const listener);

	private:
		float zoomLevel;
		std::mutex zoomMutex;
		ListenerList<Listener> listeners;
};

#endif // ZOOMFACTOR_H

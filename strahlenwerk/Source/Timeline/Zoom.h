#ifndef ZOOM_H
#define ZOOM_H

#include <juce>
#include <mutex>

class Zoom {
	public:
		Zoom();

		static Zoom& getZoom();

		operator float();
		Zoom& operator=(const float newZoomLevel);
		Zoom& operator*=(const float zoomLevelFactor);
		Zoom& operator/=(const float zoomLevelDivisor);

	private:
		float zoomLevel;
		std::mutex zoomMutex;
};

#endif // ZOOM_H

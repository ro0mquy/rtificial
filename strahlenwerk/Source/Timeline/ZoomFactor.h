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

	private:
		float zoomLevel;
		std::mutex zoomMutex;
};

#endif // ZOOMFACTOR_H

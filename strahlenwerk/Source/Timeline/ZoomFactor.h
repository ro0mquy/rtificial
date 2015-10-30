#ifndef ZOOMFACTOR_H
#define ZOOMFACTOR_H

#include <juce>

class ZoomFactor :
	public ChangeBroadcaster
{
	public:
		ZoomFactor();

		static ZoomFactor& getZoomFactor();

		operator float();
		ZoomFactor& operator=(const float newZoomLevel);
		ZoomFactor& operator*=(const float zoomLevelFactor);
		ZoomFactor& operator/=(const float zoomLevelDivisor);

		float timeToPixels(const int time);
		int pixelsToTime(const float pixels);

		float getGridWidth();
		int snapValueToGrid(const int valueAsTime);
		float getClippedFactor(const float originalZoomLevelFactor);

	private:
		float zoomLevel;
};

#endif // ZOOMFACTOR_H

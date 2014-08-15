#ifndef COLORCHANGELISTENER_H
#define COLORCHANGELISTENER_H

class ColorChangedListener {
	public:
		virtual void updateL(float L) = 0;
		virtual void updateAB(float a, float b) = 0;
};

#endif

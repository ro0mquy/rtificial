#ifndef PROJECTLISTENER_H
#define PROJECTLISTENER_H

class ProjectListener {
	public:
		virtual void postprocChanged() {}
		virtual void scenesChanged() {}
		virtual void ambientLightsChanged() {}
};

#endif

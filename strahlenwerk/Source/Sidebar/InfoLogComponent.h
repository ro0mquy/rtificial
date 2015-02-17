#ifndef INFOLOGCOMPONENT_H
#define INFOLOGCOMPONENT_H

#include <juce>
#include <Project/Project.h>

class InfoLogComponent :
	public Component,
	private Project::Listener
{
	public:
		InfoLogComponent();
		~InfoLogComponent();

		void paint(Graphics& g) override;
		void infoLogChanged() override;

	private:

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoLogComponent)
};

#endif // INFOLOGCOMPONENT_H

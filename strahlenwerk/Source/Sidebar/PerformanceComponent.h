#ifndef PERFORMANCECOMPONENT_H
#define PERFORMANCECOMPONENT_H

#include <juce>
#include <Project/Project.h>

class PerformanceComponent :
	public Component,
	private Project::Listener
{
	public:
		PerformanceComponent();
		~PerformanceComponent();

		void paint(Graphics& g) override;
		void performanceLogChanged() override;

	private:

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformanceComponent)
};

#endif // PERFORMANCECOMPONENT_H

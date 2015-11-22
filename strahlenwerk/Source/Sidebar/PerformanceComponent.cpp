#include "PerformanceComponent.h"

#include <StrahlenwerkApplication.h>
#include <Project/Project.h>

PerformanceComponent::PerformanceComponent() {
	StrahlenwerkApplication::getInstance()->getProject().addListener(this);
}

PerformanceComponent::~PerformanceComponent() {
	StrahlenwerkApplication::getInstance()->getProject().removeListener(this);
}

void PerformanceComponent::paint(Graphics& g) {
	const performance_log_t& performanceLog = StrahlenwerkApplication::getInstance()->getProject().getPerformanceLog();
	String log = "";
	for (const std::pair<std::string, uint64_t>& logEntry : performanceLog) {
		log += logEntry.first + ": " + std::to_string(logEntry.second / 1000) + " µs\n";
	}
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 13, Font::plain));
	g.drawMultiLineText(log, 20, 20, getWidth() - 40);
}

void PerformanceComponent::performanceLogChanged() {
	repaint();
}

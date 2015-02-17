#include "InfoLogComponent.h"

#include <StrahlenwerkApplication.h>
#include <Project/Project.h>

InfoLogComponent::InfoLogComponent() {
	StrahlenwerkApplication::getInstance()->getProject().addListener(this);
}

InfoLogComponent::~InfoLogComponent() {
	StrahlenwerkApplication::getInstance()->getProject().removeListener(this);
}

void InfoLogComponent::paint(Graphics& g) {
	const String& infoLog = StrahlenwerkApplication::getInstance()->getProject().getLog();
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 13, Font::plain));
	g.drawMultiLineText(infoLog, 20, 20, getWidth() - 40);
}

void InfoLogComponent::infoLogChanged() {
	repaint();
}

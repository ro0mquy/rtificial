#include "ScreenRecorder.h"

#include <MainWindow.h>
#include <StrahlenwerkApplication.h>
#include <Project/Project.h>
#include <Renderer.h>
#include <PropertyNames.h>
#include <RtImage.h>

ScreenRecorder::ScreenRecorder(Renderer& renderer) :
	screenshooter(renderer)
{
	MainWindow::getApplicationCommandManager().addListener(this);
}

void ScreenRecorder::performSaveScreenshot() {
	screenshooter.saveScreenshot();
}

void ScreenRecorder::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo& info) {
	switch (info.commandID) {
		case ScreenRecorder::saveScreenshot:
			performSaveScreenshot();
			break;
	}
}

void ScreenRecorder::applicationCommandListChanged() {
}


ScreenRecorder::Screenshot::Screenshot(Renderer& renderer_) :
	renderer(renderer_)
{
}

ScreenRecorder::Screenshot::~Screenshot() {
	renderer.removeChangeListener(this);
}

void ScreenRecorder::Screenshot::changeListenerCallback(ChangeBroadcaster* /*source*/) {
	renderer.removeChangeListener(this);
	const Image& screenshot = renderer.getRenderedImage().toJuceImage();

	String defaultScreenshotDir =
		File::getSpecialLocation(File::userHomeDirectory)
		.getChildFile("Dropbox")
		.getChildFile("rtificial")
		.getChildFile("Screenshots")
		.getFullPathName();

	const File screenshotDir(StrahlenwerkApplication::getInstance()->getProperties().getValue(PropertyNames::ScreenshotDir, defaultScreenshotDir));
	const File screenshotProjectDir(screenshotDir.getChildFile(StrahlenwerkApplication::getInstance()->getProject().getProjectName()));
	screenshotProjectDir.createDirectory();

	String screenshotBasename("Screenshot from " + Time::getCurrentTime().formatted("%F %T"));
/*
	File ppmFile(screenshotProjectDir.getChildFile(screenshotBasename + ".ppm"));
	const std::unique_ptr<FileOutputStream> ppmStream(ppmFile.createOutputStream());
	ppmStream->truncate();

	//String ppmString;
	//ppmString << "P3\n" << screenshot.getWidth() << " " << screenshot.getHeight() << "\n255\n";

	String ppmHeader;
	ppmHeader << "P6\n" << screenshot.getWidth() << " " << screenshot.getHeight() << "\n255\n";
	ppmStream->writeString(ppmHeader);

	const int height = screenshot.getHeight();
	const int width = screenshot.getWidth();
	for(int y=0; y < height; y++){
		for(int x=0; x < width; x++){
			Colour colour = screenshot.getPixelAt(x,y);
			//ppmString << int(colour.getRed()) << " " << int(colour.getGreen()) << " " << int(colour.getBlue()) << " ";
			ppmStream->writeByte(colour.getRed());
			ppmStream->writeByte(colour.getGreen());
			ppmStream->writeByte(colour.getBlue());
		}
		//ppmString << "\n";
	}
	//ppmStream->writeString(ppmString);
	ppmStream->flush();
	std::cout << "Screenshot saved as '" << ppmFile.getFullPathName() << "'\n";
*/

	const File pngFile(screenshotProjectDir.getChildFile(screenshotBasename + ".png"));
	jassert(!pngFile.exists());
	const std::unique_ptr<FileOutputStream> pngStream(pngFile.createOutputStream());
	pngStream->truncate();
	PNGImageFormat png;
	bool success = png.writeImageToStream(screenshot, *pngStream);
	pngStream->flush();
	if (success) {
		std::cout << "Screenshot saved as '" << pngFile.getFullPathName() << "'\n";
		Desktop::getInstance().getDefaultLookAndFeel().playAlertSound();
	}
}

void ScreenRecorder::Screenshot::saveScreenshot() {
	renderer.addChangeListener(this);
	renderer.requestRenderedImage();
}


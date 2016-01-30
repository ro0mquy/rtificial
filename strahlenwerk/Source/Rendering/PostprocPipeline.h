#ifndef POSTPROCPIPELINE_H
#define POSTPROCPIPELINE_H

#define GL_GLEXT_PROTOTYPES
#include "glcorert.h"

#include <vector>
#include <memory>
#include <mutex>

#include <juce>

#include <RtImage.h>

class PostprocShader;
class SceneShader;

class PostprocPipeline :
	public ChangeBroadcaster,
	private AsyncUpdater
{
	public:
		~PostprocPipeline();

		void handleAsyncUpdate() override;

		void setShaders(std::vector<std::unique_ptr<PostprocShader>> shaders);
		uint64_t render(SceneShader& shader, int width, int height, bool shouldGetImage = false);
		PostprocShader& getShader(int n) const;
		int getNumShaders() const;

		void requestRenderedImage();
		const RtImage& getRenderedImage();

	private:
		std::vector<std::unique_ptr<PostprocShader>> shaders;
		std::vector<GLuint> queries;

		bool shouldGetImage;
		ScopedPointer<float> pixelData;
		int pixelDataWidth;
		int pixelDataHeight;
		const int pixelDataChannels = 3;
		std::mutex pixelDataMutex;
		void readPixels(int width, int height);
		RtImage renderedImage;
};

#endif

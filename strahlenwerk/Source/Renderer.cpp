#include <fstream>
#include <sstream>
#include <regex>

#include "Renderer.h"

#include "Rendering/PostprocShader.h"
#include "StrahlenwerkApplication.h"
#include "Rendering/PostprocPipeline.h"
#include "Rendering/PostprocPipelineLoader.h"

Renderer::Renderer(OpenGLContext& context) :
	context(context),
	defaultShader(context),
	defaultPostproc(new PostprocPipeline),
	width(0),
	height(0)
{
	defaultShader.load("float rect_mask(vec2 p,vec2 center,vec2 dimensions){return (1.-step(dimensions.x,length(p.x-center.x)))*(1.-step(dimensions.y,length(p.y-center.y)));}float circle_mask(vec2 p,vec2 center,float r){return 1.-step(r,length(p-center));}void main(void){vec2 p=gl_FragCoord.xy/vec2(600.,400.);vec2 center=vec2(0.5);vec3 color=vec3(0.);float the_cirle=circle_mask(p,center,0.475);float not_the_circle=1.-the_cirle;color+=vec3(0.3) * not_the_circle;color+=(floor(pow(sin((p.x+.05)*10.*3.141),2.)+0.01)+floor(pow(sin((p.y+.05)*10.*3.141),2.)+0.01))*(1.-(the_cirle*(1.-rect_mask(p,vec2(.5,.5),vec2(.5,.05)))));color+=vec3(1.)*the_cirle*rect_mask(p,vec2(.15,.9),vec2(.2,.05));color+=vec3(1.)*the_cirle*rect_mask(p,vec2(.85,.9),vec2(.2,.05));color+=vec3(1.)*the_cirle*rect_mask(p,vec2(.5,1.),vec2(.5,.05));color+=.5*mod(floor(p.x*10.),2.)*the_cirle*rect_mask(p,vec2(.5,.8),vec2(.5,.05));color+=vec3(.5,.5,0.)*the_cirle*rect_mask(p,vec2(.0,.65),vec2(.1,.1));color+=vec3(.0,.5,.5)*the_cirle*rect_mask(p,vec2(.2,.65),vec2(.1,.1));color+=vec3(0.,1.,0.)*the_cirle*rect_mask(p,vec2(.4,.65),vec2(.1,.1));color+=vec3(.5,.0,.5)*the_cirle*rect_mask(p,vec2(.6,.65),vec2(.1,.1));color+=vec3(1.,0.,0.)*the_cirle*rect_mask(p,vec2(.8,.65),vec2(.1,.1));color+=vec3(0.,0.,1.)*the_cirle*rect_mask(p,vec2(1.,.65),vec2(.1,.1));float c=p.x+.3;color+=sin(c*3.14*200.*log(c))*the_cirle*rect_mask(p,vec2(.5,.35),vec2(0.5,0.1));color+=floor(p.x*6.)/6.* the_cirle*rect_mask(p,vec2(.5,.2),vec2(.5,.05));color+=vec3(.5,.5,0.)*the_cirle*rect_mask(p,vec2(.25,.05),vec2(.2,.1));color+=vec3(.5,.5,0.)*the_cirle*rect_mask(p,vec2(.75,.05),vec2(.2,.1));color+=vec3(1.,0.,0.)*the_cirle*rect_mask(p,vec2(.5,.05),vec2(.05,.1));gl_FragColor.rgb=color;gl_FragColor.a=0.;}");
	PostprocPipelineLoader loader;
	defaultPostproc->setShaders(loader.load(context, "input.color output.color\n", {}));
	postprocMutex.lock();
	postproc = std::move(defaultPostproc);
	postprocMutex.unlock();

	auto& project = StrahlenwerkApplication::getInstance()->getProject();
	project.registerPostprocListener(this);
	project.contextChanged(context);
}

Renderer::~Renderer() {
	StrahlenwerkApplication::getInstance()->getProject().unregisterPostprocListener(this);
}

void Renderer::newOpenGLContextCreated() {
}

void Renderer::openGLContextClosing() {
}

void Renderer::renderOpenGL() {
	postprocMutex.lock();
	postproc->render(defaultShader, width, height);
	postprocMutex.unlock();
}

void Renderer::changeListenerCallback(ChangeBroadcaster* source) {
	reloadPostproc();
}

void Renderer::setSize(int _width, int _height) {
	postprocMutex.lock();
	width = _width;
	height = _height;
	postprocMutex.unlock();
}

void Renderer::reloadPostproc() {
	auto newPostproc = StrahlenwerkApplication::getInstance()->getProject().getPostproc();
	postprocMutex.lock();
	if(newPostproc != nullptr) {
		if(defaultPostproc == nullptr) {
			defaultPostproc = std::move(postproc);
		}
		postproc = std::move(newPostproc);
	} else {
		if(defaultPostproc != nullptr) {
			postproc = std::move(defaultPostproc);
		}
	}
	postprocMutex.unlock();
	context.triggerRepaint();
}

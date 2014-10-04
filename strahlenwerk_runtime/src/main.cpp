#include "Backend.h"

#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "strahlenwerk_export_interface.h"
#include "glcorearb.h"
#include "gl_identifiers.h"
#include "shaders/ladebalken.h"

#ifdef BUILD_LINUX
	using Backend = LinuxBackend;

#elif _WINDOWS
	// las said this was good
#	define VC_EXTRALEAN
#	define WIN32_LEAN_AND_MEAN

#	include <windows.h>
#	include <GL/gl.h>

	using Backend = WindowsBackend;
#endif

	typedef unsigned int uint;

	void* __cdecl operator new(uint bytes){
		return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
	}

		void* __cdecl operator new [](uint bytes) {
		return operator new(bytes);
	}

		void* __cdecl operator new(uint bytes, void* here){
		return here;
	}

		void __cdecl operator delete(void* ptr) {
		if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
	}

	void __cdecl operator delete [](void* ptr) {
		operator delete(ptr);
	}

	typedef void (__cdecl *ePVFV)();




#pragma data_seg(".CRT$XCA")
	ePVFV __xc_a[] = { nullptr };
#pragma data_seg(".CRT$XCZ")
	ePVFV __xc_z[] = { nullptr };
#pragma data_seg() // reset data segment

	static const unsigned int eMAX_ATEXITS = 32;
	static ePVFV g_atExitList[eMAX_ATEXITS];

	void memset2(void * dst, unsigned char val, unsigned int count)
	{
		__asm
		{
			mov     eax, dword ptr[val]
				mov     ecx, dword ptr[count]
				mov     edi, dword ptr[dst]
				rep     stosb
		}
	}

	static void initTerm(ePVFV *pfbegin, ePVFV *pfend)
	{
		while (pfbegin < pfend)
		{
			if (*pfbegin)
				(**pfbegin)();

			pfbegin++;
		}
	}

	static void initAtExit()
	{
		memset2(g_atExitList, 0, sizeof(g_atExitList));
	}

	static void doAtExit()
	{
		initTerm(g_atExitList, g_atExitList + eMAX_ATEXITS);
	}

	int __cdecl atexit(ePVFV func)
	{
		// get next free entry in atexist list
		unsigned int index = 0;
		while (g_atExitList[index++]);
	//	eASSERT(index < eMAX_ATEXITS);

		// put function pointer to destructor there
		if (index < eMAX_ATEXITS)
		{
			g_atExitList[index] = func;
			return 0;
		}

		return -1;
	}

		extern "C" int _cdecl _purecall(void)
	{
		return 0;
}
		void eGlobalsStaticsInit()
		{
#ifdef NDEBUG
			initAtExit();
			initTerm(__xc_a, __xc_z); 
#endif
		}

		void eGlobalsStaticsFree()
		{
#ifdef NDEBUG
			doAtExit();
#endif
		}

#ifdef _DEBUG
#ifdef _WINDOWS
#	include <string>
#endif
#endif

const int width = 1920;
const int height = 1080;
const bool fullscreen = true;
const bool use_sound_thread = true;


#ifdef _WINDOWS
#	ifdef _DEBUG
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#	else
void WinMainCRTStartup(){
	eGlobalsStaticsInit();
#	endif
#else
	int main() {
#endif

	// init backend
	Backend backend;
	backend.init(width, height, fullscreen);
	backend.initAudio(use_sound_thread);

	const int n_scenes = sizeof(scenes) / sizeof(Shader);
	const int n_postproc = sizeof(postproc) / sizeof(Shader);

	// TODO
	// musik
	// ladebalken/precalc

	ladebalken.compile();

	float progress = 0.;
	float progress_step = 1./ (n_scenes + n_postproc);

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].compile();
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1.);
		backend.afterFrame();
		Sleep(100);
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].compile();
		fbos[i].create(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1.);
		backend.afterFrame();
		Sleep(100);
	}

	backend.playAudio();

	int scene_id = 0;
	int shader_id = scenes_data[scene_id].sceneId;
	const int last_scene_id = sizeof(scenes_data) / sizeof(Scene) - 1;
	while(backend.beforeFrame()) {
		if(scenes_data[scene_id].end < backend.getTime()) {
			if(scene_id == last_scene_id) {
				break;
			} else {
				scene_id++;
				shader_id = scenes_data[scene_id].sceneId;
			}
		}

		const double currentTime = backend.getTime();

		fbos[0].bind();
		scenes[shader_id].draw(fbos[0].width, fbos[0].height, currentTime);
		fbos[0].unbind();
		for(int i = 0; i < n_postproc - 1; i++) {
			fbos[i + 1].bind();
			postproc[i].draw(fbos[i + 1].width, fbos[i + 1].height, currentTime);
			fbos[i + 1].unbind();
		}

		glEnable(GL_FRAMEBUFFER_SRGB);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		postproc[n_postproc - 1].draw(width, height, currentTime);
		glDisable(GL_FRAMEBUFFER_SRGB);
		backend.afterFrame();

#		ifdef _DEBUG && _WINDOWS
		OutputDebugString((std::to_string(backend.getTime()) + "\n").c_str());
#		endif
	}

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].destroy();
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].destroy();
		fbos[i].destroy();
	}

	backend.cleanup();
	eGlobalsStaticsFree();
	ExitProcess(0);
#	ifdef _DEBUG
	return 0;
#	endif
}

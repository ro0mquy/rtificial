#ifdef _WINDOWS

// http://stackoverflow.com/questions/1583196/building-visual-c-app-that-doesnt-use-crt-functions-still-references-some
extern "C" {
	int _fltused = 1;
}

#include "Frontend.h"
#include "glcorearb.h"
#include "strahlenwerk_export_interface.h"

PFNGLACTIVETEXTUREPROC            glActiveTexture;
PFNGLGENBUFFERSPROC               glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC          glBindFramebuffer;
PFNGLGENERATEMIPMAPPROC           glGenerateMipmap;
PFNGLFRAMEBUFFERTEXTURE2DPROC     glFramebufferTexture2D;
PFNGLDRAWBUFFERSPROC              glDrawBuffers;
PFNGLDELETEFRAMEBUFFERSPROC       glDeleteFramebuffers;
PFNGLCREATESHADERPROC             glCreateShader;
PFNGLSHADERSOURCEPROC             glShaderSource;
PFNGLCOMPILESHADERPROC            glCompileShader;
PFNGLCREATEPROGRAMPROC            glCreateProgram;
PFNGLATTACHSHADERPROC             glAttachShader;
PFNGLDELETESHADERPROC             glDeleteShader;
PFNGLLINKPROGRAMPROC              glLinkProgram;
PFNGLUSEPROGRAMPROC               glUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
PFNGLUNIFORM1FPROC                glUniform1f;
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLUNIFORM3FPROC                glUniform3f;
PFNGLUNIFORM4FPROC                glUniform4f;
PFNGLUNIFORM1IPROC                glUniform1i;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLDEBUGMESSAGECONTROLPROC      glDebugMessageControl;
PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;


void WindowsFrontend::init(int width, int height, bool fullscreen) {
	DEVMODE device_mode = {
		"", 0, 0, sizeof(device_mode), 0, DM_PELSWIDTH | DM_PELSHEIGHT, { { 0, 0, 0, 0, 0, 0, 0, 0 } }, 0, 0, 0, 0, 0,
		"", 0, 0, width, height, { 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	PIXELFORMATDESCRIPTOR pixel_format_descriptor =	{
		sizeof(pixel_format_descriptor), 1, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0
	};

	// open Windows window
	if (fullscreen) {
		ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN);
	}

	window_handle = GetDC(CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE , 0, 0, 0, 0, 0, 0, 0, 0));
	SetPixelFormat(window_handle, ChoosePixelFormat(window_handle, &pixel_format_descriptor), &pixel_format_descriptor);
	wglMakeCurrent(window_handle, wglCreateContext(window_handle));
	ShowCursor(FALSE);

	// Load OpenGL functions manually
	glActiveTexture            = (PFNGLACTIVETEXTUREPROC)            wglGetProcAddress("glActiveTexture");
	glGenFramebuffers          = (PFNGLGENBUFFERSPROC)               wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer          = (PFNGLBINDFRAMEBUFFERPROC)          wglGetProcAddress("glBindFramebuffer");
	glGenerateMipmap           = (PFNGLGENERATEMIPMAPPROC)           wglGetProcAddress("glGenerateMipmap");
	glFramebufferTexture2D     = (PFNGLFRAMEBUFFERTEXTURE2DPROC)     wglGetProcAddress("glFramebufferTexture2D");
	glDrawBuffers              = (PFNGLDRAWBUFFERSPROC)              wglGetProcAddress("glDrawBuffers");
	glDeleteFramebuffers       = (PFNGLDELETEFRAMEBUFFERSPROC)       wglGetProcAddress("glDeleteFramebuffers");
	glCreateShader             = (PFNGLCREATESHADERPROC)             wglGetProcAddress("glCreateShader");
	glShaderSource             = (PFNGLSHADERSOURCEPROC)             wglGetProcAddress("glShaderSource");
	glCompileShader            = (PFNGLCOMPILESHADERPROC)            wglGetProcAddress("glCompileShader");
	glCreateProgram            = (PFNGLCREATEPROGRAMPROC)            wglGetProcAddress("glCreateProgram");
	glAttachShader             = (PFNGLATTACHSHADERPROC)             wglGetProcAddress("glAttachShader");
	glDeleteShader             = (PFNGLDELETESHADERPROC)             wglGetProcAddress("glDeleteShader");
	glLinkProgram              = (PFNGLLINKPROGRAMPROC)              wglGetProcAddress("glLinkProgram");
	glUseProgram               = (PFNGLUSEPROGRAMPROC)               wglGetProcAddress("glUseProgram");
	glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)  wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)      wglGetProcAddress("glVertexAttribPointer");
	glUniform1f                = (PFNGLUNIFORM1FPROC)                wglGetProcAddress("glUniform1f");
	glUniform2f                = (PFNGLUNIFORM2FPROC)                wglGetProcAddress("glUniform2f");
	glUniform3f                = (PFNGLUNIFORM3FPROC)                wglGetProcAddress("glUniform3f");
	glUniform4f                = (PFNGLUNIFORM4FPROC)                wglGetProcAddress("glUniform4f");
	glUniform1i                = (PFNGLUNIFORM1IPROC)                wglGetProcAddress("glUniform1i");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
	glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)            wglGetProcAddress("glDeleteProgram");
	glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)             wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)        wglGetProcAddress("glGetProgramInfoLog");
	glGetShaderiv              = (PFNGLGETSHADERIVPROC)              wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)         wglGetProcAddress("glGetShaderInfoLog");
	glDebugMessageControl      = (PFNGLDEBUGMESSAGECONTROLPROC)      wglGetProcAddress("glDebugMessageControl");
	glDebugMessageCallback     = (PFNGLDEBUGMESSAGECALLBACKPROC)     wglGetProcAddress("glDebugMessageCallback");

	/* adaptive vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(-1);
	*/
}

#if defined(SYNTH_4KLANG) || defined(SYNTH_DUAL_V2_4KLANG)
//#include <mmsystem.h>
//#include <mmreg.h>
#include "../music/soundtrack.4klang.h"
#include "math/stdmath.h"
#define AUDIO_CHANNELS 2
#ifndef WINDOWS_OBJECT
#	error "4klang object type does not match target architecture"
#endif

static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * AUDIO_CHANNELS];
#endif

#if defined(SYNTH_V2) || defined(SYNTH_DUAL_V2_4KLANG)
// use GetTickCount() instead of V2 for time
// #define SYSTEM_TIME
#include "v2mplayer.h"
#include "libv2.h"
static V2MPlayer player;
extern "C" const sU8 soundtrack[];
#ifdef SYSTEM_TIME
	DWORD starttime;
#endif
	long startPosition;
#endif

#ifdef SYNTH_VORBIS
#include "stb_vorbis.h"
static stb_vorbis *vorbis_decoder;
extern "C" const unsigned char soundtrack[];
extern "C" int soundtrack_size;
#define SAMPLE_TYPE short
static int SAMPLE_RATE = 44100;
static int AUDIO_CHANNELS = 2;
static int MAX_SAMPLES = 0;
static void _vorbis_decode();
static SAMPLE_TYPE *audio_buffer;
#endif

#ifdef SYNTH_DUAL_V2_4KLANG
sU32 sample_position = 0;
sU32 tempAlreadyProcessed = 0;
sU32 alreadyTruncated(0u);
sU32 alreadyProcessed(0u);
//define these
const sU32 compensation (1174);
const sU32 lengthOfV2(9200279);

float envelope[2] = {};
float envelopeLimiter[2] = {};

void __stdcall dualV2And4KlangProxy(void *a_this, sF32 *a_buffer, sU32 a_len) {
  V2MPlayer* player = reinterpret_cast<V2MPlayer*>(a_this);
	if (player->IsPlaying()) {
    sF32* a_buffer_ = a_buffer; //Necessary later for comp
    sU32 a_len_ = a_len; //Necessary later for comp
    //Only truncate every once in a while
    //via: https://www.ee.columbia.edu/~dpwe/papers/HejMus91-solafs.pdf 
    // & https://www.ee.columbia.edu/~dpwe/e4896/lectures/E4896-L09.pdf
    if (tempAlreadyProcessed > 15 * lengthOfV2 / compensation)
    {
      sU32 lenFirstWindow = a_len % 2 == 0 ? a_len / 2u : (a_len - 1u) / 2u;
      sU32 lenSecondWindow = a_len % 2 == 0 ? a_len / 2u : ((a_len - 1u) / 2u) + 1u;

      //Get Number Of Samples for This truncation iteration
      //Float (actual divion result) of samples to be cut at this exact point
      float toBeTruncated = (float)tempAlreadyProcessed / ((float)lengthOfV2 / (float)compensation);
      int willBeTruncated = (int)toBeTruncated;
      //This is number of smaples that should already have been cut
      float shouldBeTruncatedTillNow = alreadyProcessed / (lengthOfV2 / compensation);
      //Adjust according to prediciton value
      if (alreadyTruncated > shouldBeTruncatedTillNow) 
      {
        willBeTruncated -= 1;
      }

      sU32 windowOverlapp = (sU32)willBeTruncated;
      alreadyTruncated += willBeTruncated;

      //Play V2 To Buffer
      player->Render(a_buffer, a_len, 0);
      sU32 iter = lenFirstWindow - windowOverlapp;
      for (int j = 0; j < windowOverlapp; j++)
      {
        //Fade out
        float multiplier = 1 - (float)(j+1) / (float(windowOverlapp + 1));
        a_buffer[2 * iter] *= multiplier;
        a_buffer[2 * iter + 1] *= multiplier;
        
        //Fade in
        multiplier = (float)(j + 1)/ (float(windowOverlapp + 1));
        a_buffer[2 * iter] += multiplier * a_buffer[2 * (lenFirstWindow + j)];
        a_buffer[2 * iter + 1] += multiplier * a_buffer[2 * (lenFirstWindow + j) + 1];

        iter++;

      }
      //Render remainders of second window
      for (int i = lenFirstWindow + windowOverlapp; i < a_len; i++)
      {
        a_buffer[2 * (i - windowOverlapp)] = a_buffer[2*i];
        a_buffer[2 * (i - windowOverlapp) + 1] = a_buffer[2*i+1];
      }

      //Adjust a_len
      a_len -= windowOverlapp;

      //Get4Klang
      sU32 position = sample_position * 2;
      // TODO assumes floating point samples
      for (sU32 i = 0; i < a_len; i++) {
        a_buffer[2 * i] += audio_buffer[position + 2 * i];
        a_buffer[2 * i + 1] += audio_buffer[position + 2 * i + 1];
      }

      //Write a_buffer till a_len
      a_buffer += a_len * 2u;
      sample_position += a_len;
      position = sample_position * 2;
      player->Render(a_buffer, windowOverlapp, 0);
      for (sU32 i = 0; i < windowOverlapp; i++) {
        a_buffer[2 * i] += audio_buffer[position + 2 * i];
        a_buffer[2 * i + 1] += audio_buffer[position + 2 * i + 1];
      }
      sample_position += windowOverlapp;
      a_buffer += 2 * windowOverlapp;

      //Clean Up
      tempAlreadyProcessed = 0;
    }
    else
    {
      //Get4Klang
      sU32 position = sample_position * 2;
      // TODO assumes floating point samples
      for (sU32 i = 0; i < a_len; i++) {
        a_buffer[2 * i] = audio_buffer[position + 2 * i];
        a_buffer[2 * i + 1] = audio_buffer[position + 2 * i + 1];
      }
      player->Render(a_buffer, a_len, 1);
      tempAlreadyProcessed += a_len;
      //Clean Up
      a_buffer += a_len * 2u;
      sample_position += a_len;
    }

	// TODO compressor and limiter
    bool compressorOn = true;
    if (compressorOn)
    {
		  float envelope[2]{};
		  float attack_ = 10.f;
		  float release_ = 100.f;
		  float attackGain = exp(-1 / (attack_ * 44100));
		  float releaseGain = exp(-1 / (release_ * 44100));
		  float threshold_ = -15.f;
		  float ratio_ = 4;
		  float slope = 1 - (1 / ratio_);
		  for (int i = 0; i < a_len_; i++) {
			for (int channel = 0; channel < 2u; channel++) {
				sF32* channelData = a_buffer_;
				//float input = abs(channelData[2 * i + channel]);
				float input = (channelData[2 * i + channel]) < 0.f ? -1.f * (channelData[2 * i + channel]) : (channelData[2 * i + channel]);
				if (envelope[channel] < input) {
					envelope[channel] = input + attackGain * (envelope[channel] - input);
				}
				else {
					envelope[channel] = input + releaseGain * (envelope[channel] - input);
				}
			}
			float envelopeValue = max(envelope[0], envelope[1]);
			for (int channel = 0; channel < 2u; channel++) {
				sF32* channelData = a_buffer_;
				float input = channelData[i * 2 + channel] < 0.f ? -1.f * channelData[i * 2 + channel] : channelData[i * 2 + channel];
				float inputDb = 20 * log10(input);
				float gainDb = min(0.f, slope * (threshold_ - inputDb));
				channelData[2 * i + channel] *= pow(10.f, gainDb / 20.f);
				channelData[2 * i + channel] *= pow(10.f, 5.5f / 10.f);
			}
		}
    }
    /*bool limiterOn = false;
    if (limiterOn)
    {
      //Now, lots of code for circular buffer

      class CircularBuffer
      {
      private:
        float* buffer;
        size_t numberOfSamples;
        float* minMaxStore[4]; // min, max, absmin, absmax
      public:
        CircularBuffer(size_t numberOfSamples_) : buffer(nullptr), numberOfSamples(numberOfSamples_)
        {
          buffer = new float[numberOfSamples];
          for (int i = 0; i < numberOfSamples; i++)
          {
            buffer[i] = float();
          }
          for (int i = 0; i < 2; i++) minMaxStore[i] = &buffer[0];
        };
        ~CircularBuffer(void)
        {
          if (this->buffer != nullptr)
          {
            delete[] buffer;
            buffer = nullptr;
          }
        };
        void push(float const& in)
        {
          for (int i = 0; i < numberOfSamples - 1u; i++)
          {
            buffer[i + 1u] = buffer[i];
          }
          buffer[0] = in;
          for (int i = 0; i < 4; i++)
          {
            minMaxStore[i]++;
            if (minMaxStore[i] > &buffer[numberOfSamples - 1u])
            {
              if (i == 0) this->searchMin();
              else if (i == 1) this->searchMax();
              else if (i == 2) this->searchAbsMin();
              else if (i == 3) this->searchAbsMax();
            }
          }
        };
        float pushpop(float const& in)
        {
          float holder = buffer[numberOfSamples - 1u];
          for (int i = 0; i < numberOfSamples - 1u; i++)
          {
            buffer[i + 1u] = buffer[i];
          }
          buffer[0] = in;

          for (int i = 0; i < 4; i++)
          {
            minMaxStore[i]++;
            if (minMaxStore[i] > &buffer[numberOfSamples - 1u])
            {
              if (i == 0) this->searchMin();
              else if (i == 1) this->searchMax();
              else if (i == 2) this->searchAbsMin();
              else if (i == 3) this->searchAbsMax();
            }
          }
          return holder;
        };
        float getMin() {
          return *minMaxStore[0];
        };
        float getAbsMax() {
          return (*minMaxStore[3] > 0.f ? *minMaxStore[3] : -1.f * (*minMaxStore[3]));
        };
        float getAbsMin() {
          return (*minMaxStore[2] > 0.f ? *minMaxStore[2] : -1.f * (*minMaxStore[2]));
        };
        float getMax() {
          return *minMaxStore[1];
        };
        void searchMin() {
          float* minimum = &buffer[0];
          for (int i = 1; i < numberOfSamples; i++)
          {
            if (buffer[i] < *minimum)
            {
              minimum = &buffer[i];
            }
          }
          minMaxStore[0] = minimum;
        };
        void searchMax() {
          float* maximum = &buffer[0];
          for (int i = 1; i < numberOfSamples; i++)
          {
            if (buffer[i] > *maximum)
            {
              maximum = &buffer[i];
            }
          }
          minMaxStore[1] = maximum;
        };
        void searchAbsMin() {
          float min = buffer[0] > 0.f ? buffer[0] : -1.f * buffer[0];
          float* ptr = &buffer[0];
          for (int i = 1; i < numberOfSamples; i++)
          {
            float temp = buffer[i] > 0.f ? buffer[i] : -1.f * buffer[i];
            if (temp < min)
            {
              ptr = &buffer[i];
              min = temp;
            }
          }
          minMaxStore[2] = ptr;
        };
        void searchAbsMax() {
          float maximum = buffer[0] > 0.f ? buffer[0] : -1.f * buffer[0];
          float* ptr = &buffer[0];
          for (int i = 1; i < numberOfSamples; i++)
          {
            float temp = buffer[i] > 0.f ? buffer[i] : -1.f * buffer[i];
            if (temp > maximum)
            {
              ptr = &buffer[i];
              maximum = temp;
            }
          }
          minMaxStore[3] = ptr;
        };
        float getSum() {
          float summed;
          for (int i = 0; i < numberOfSamples; i++) summed += buffer[i];
          return summed;
        };
      };

      sF32* channelData = a_buffer_;
      float release_ = 100;
      float igain = 1.45f;
      float ogain = 0.45f;
      CircularBuffer longCircularBuffer(400);
      CircularBuffer shortCircularBuffer1(200u), shortCircularBuffer2(200u);
      for (int i = 0; i < a_len_; i++) {
        for (int channel = 0; channel < 2; channel++)
        {
          float current = longCircularBuffer.pushpop(channelData[2 * i + channel] * igain);
          float max = longCircularBuffer.getAbsMax() > (current > 0.f ? current : -1.f * current) ? longCircularBuffer.getAbsMax() : (current > 0.f ? current : -1.f * current);
          float gainReduction = max > 1.f ? 1.f / max : 1.f;
          shortCircularBuffer1.push(gainReduction);
          float sCB1_normsum = shortCircularBuffer1.getSum() / 200.f;
          shortCircularBuffer2.push(sCB1_normsum);
          float sCB2_normsum = shortCircularBuffer2.getSum() / 200.f;
          if (sCB2_normsum > gainReduction * release_ / 500.f)
          {
            gainReduction = sCB2_normsum;
          }
          current *= ogain;
          channelData[2 * i + channel] = current * gainReduction;
        }
      }
    }*/
	}
}
#endif

#if defined(SYNTH_BLANKENHAIN)
#include "blankenhain.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#define FLOAT_32BIT
#define SAMPLE_TYPE float
#define AUDIO_CHANNELS 2
#define SAMPLE_RATE 44100
static int MAX_SAMPLES = 0;
static SAMPLE_TYPE* audio_buffer;
#endif

void WindowsFrontend::initAudio(bool threaded) {
#if defined(SYNTH_4KLANG) || defined(SYNTH_DUAL_V2_4KLANG)
	if (threaded) {
		// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) _4klang_render, audio_buffer, 0, 0);
	} else {
		_4klang_render(audio_buffer);
	}
#endif

#if defined(SYNTH_V2) || defined(SYNTH_DUAL_V2_4KLANG)
	player.Init();
	player.Open(soundtrack);
#ifdef SYNTH_V2
	dsInit(player.RenderProxy, &player, GetForegroundWindow());
#endif
#ifdef SYNTH_DUAL_V2_4KLANG
	dsInit(dualV2And4KlangProxy, &player, GetForegroundWindow());
#endif
#endif

#ifdef SYNTH_VORBIS
	int vorbis_error = 0;
	stb_vorbis *vorbis_decoder = stb_vorbis_open_memory(soundtrack, soundtrack_size, &vorbis_error, NULL);
	stb_vorbis_info vorbis_info = stb_vorbis_get_info(vorbis_decoder);

	#ifdef _DEBUG
		vorbis_error = stb_vorbis_get_error(vorbis_decoder);
		RT_DEBUG(("vorbis error code: " + std::to_string(vorbis_error) + "\n").c_str());
		RT_DEBUG(("channels:          " + std::to_string(vorbis_info.channels) + "\n").c_str());
		RT_DEBUG(("sample_rate:       " + std::to_string(vorbis_info.sample_rate) + "\n").c_str());
		RT_DEBUG(("predicted memory usage:\n"
				"  total: " + std::to_string(vorbis_info.setup_memory_required + vorbis_info.temp_memory_required) + "\n" +
				"  setup: " + std::to_string(vorbis_info.setup_memory_required) + "\n" +
				"  temp:  " + std::to_string(vorbis_info.temp_memory_required) + "\n"
			).c_str());
	#endif

	AUDIO_CHANNELS = vorbis_info.channels;
	SAMPLE_RATE = vorbis_info.sample_rate;

	stb_vorbis_close(vorbis_decoder);

	/*
	 * don't thread because we need the number of decoded threads for the WAVEHDR
	 */
//	if (threaded) {
//		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) _vorbis_decode, NULL, 0, 0);
//	} else {
		_vorbis_decode();
//	}
#endif

#if defined(SYNTH_BLANKENHAIN)
	MAX_SAMPLES = blankenhain::lengthInSamples();
	audio_buffer = new SAMPLE_TYPE[MAX_SAMPLES * AUDIO_CHANNELS];

	if (threaded) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)blankenhain::render, audio_buffer, 0, 0);
		// wait one second
		sleep(1000);
	} else {
		blankenhain::render(audio_buffer);
	}
#endif

#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS) || defined(SYNTH_BLANKENHAIN)
	// This should've been defined somewhere. No clue why it wasn't.
	// http://www.onicos.com/staff/iz/formats/wav.html
	#define WAVE_FORMAT_IEEE_FLOAT		0x0003 /* IEEE Float */

	audio_wave_header = {
		(LPSTR)audio_buffer,
		MAX_SAMPLES * sizeof(SAMPLE_TYPE) * AUDIO_CHANNELS,
		0,
		0,
		0,
		0,
		0,
		0
	};

	WAVEFORMATEX wave_format = {
#		ifdef FLOAT_32BIT	
		WAVE_FORMAT_IEEE_FLOAT,
#		else
		WAVE_FORMAT_PCM,
#		endif		
		/* channels        */ AUDIO_CHANNELS,
		/* samples/second  */ SAMPLE_RATE,
		/* bytes/second    */ SAMPLE_RATE*sizeof(SAMPLE_TYPE) * AUDIO_CHANNELS,
		/* block alignment */ sizeof(SAMPLE_TYPE) * AUDIO_CHANNELS,
		/* bits/sample     */ sizeof(SAMPLE_TYPE) * 8,
		/* no extensions   */ 0
	};
#endif

#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS) || defined(SYNTH_BLANKENHAIN)
	waveOutOpen(&audio_wave_out, WAVE_MAPPER, &wave_format, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));
#endif
}

#ifdef SYNTH_VORBIS
static void _vorbis_decode() {
	/* use the global audio_buffer here, the param is just for matching the 4klang call */
	MAX_SAMPLES = stb_vorbis_decode_memory(soundtrack, soundtrack_size, &AUDIO_CHANNELS, &SAMPLE_RATE, &audio_buffer);
#ifdef _DEBUG
	RT_DEBUG(("number of samples decoded: " + std::to_string(MAX_SAMPLES) + "\n").c_str());
	RT_DEBUG(("channels:                  " + std::to_string(AUDIO_CHANNELS) + "\n").c_str());
	RT_DEBUG(("sample_rate:               " + std::to_string(SAMPLE_RATE) + "\n").c_str());
	#endif
}
#endif


void WindowsFrontend::playAudio() {
#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS) || defined(SYNTH_BLANKENHAIN)

	waveOutWrite(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));
#endif
#if defined(SYNTH_V2) || defined(SYNTH_DUAL_V2_4KLANG)
	#ifdef SYSTEM_TIME
		starttime = GetTickCount();
	#endif
		startPosition = dsGetCurSmp();
	player.Play();
#endif
}


// returns time in milli beats
int WindowsFrontend::getTime(){
#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS) || defined(SYNTH_BLANKENHAIN)
	MMTIME time;
	time.wType = TIME_SAMPLES;
	waveOutGetPosition(audio_wave_out, &time, sizeof(MMTIME));
	return int(.5 + (double) time.u.sample / SAMPLE_RATE * BPM / 60. * 1000.);
#endif
#if defined(SYNTH_V2) || defined(SYNTH_DUAL_V2_4KLANG)
	#ifdef SYSTEM_TIME
		return int(.5 + double(GetTickCount() - starttime) * BPM / 60.);
	#else
		return int(.5 + (dsGetCurSmp() - startPosition) / (44.100 * sizeof(float))) * (BPM / 60.);
	#endif
#endif
}


bool WindowsFrontend::checkMessageLoop() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		//TranslateMessage( &msg ); // Don't have any keys that need translating to WM_CHAR messages
		DispatchMessage(&msg);
	}

	return !GetAsyncKeyState(VK_ESCAPE);
}


void WindowsFrontend::beforeFrame() {
}

void WindowsFrontend::afterFrame() {
	SwapBuffers(window_handle);
}


void WindowsFrontend::sleep(int milliseconds) {
	Sleep(milliseconds);
}


void WindowsFrontend::cleanup() {
#ifdef SYNTH_V2
	dsClose();
	player.Close();
#endif
#ifdef SYNTH_BLANKENHAIN
	delete audio_buffer;
#endif
}

#endif

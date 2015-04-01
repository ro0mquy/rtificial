# Strahlenwerk Runtime

## Building
### Linux
*it just doesn't work*

### Windows
Follow the installation manual for Visual Studio in the [Strahlenwerk ReadMe.md](../strahlenwerk/ReadMe.md).

Please note that the Debug Target Configuration links against the default libraries and is not suitable for submitting a build and running on a compo machine. Use the Release Target Configuration for a build that needs to run on a compo machine.

### Using the 4klang synth
Place the 4klang export into `rtificial/strahlenwerk_runtime/src/music/4klang.windows.h` and `rtificial/strahlenwerk_runtime/src/music/4klang.windows.obj`.

Be sure to include `winmm.lib` and `4klang.windows.obj` in *Properties* > *Configuration Properties* >*Linker* > *Input* >*Additional Dependencies* in Visual Studio.

Define `SYNTH_4KLANG` preprocessor-define in `WindowsBackend.cpp`.

#### Using the V2 synth
> Download the `libv2` from [pouet](http://www.pouet.net/prod.php?which=15073) and unpack it. Copy `libv2/libv2.h`, `libv2/v2mplayer.h`, and `libv2/v2mplayer.cpp` to `rtificial/strahlenwerk_runtime/Lib/include/` and `libv2/libv2.lib` to `rtificial/strahlenwerk_runtime/Lib/lib/`.

*`libv2` is now included in the source tree.*

It may be the cases that you need a `nasm.exe` executable. Download a standalone version (not the installer) from [nasm.us](http://nasm.us/) and extract it. Now copy the executable `nasm/nasm.exe` to `rtificial/strahlenwerk_runtime/VisualStudio2013/nasm.exe`

Place the V2 export into `rtificial/strahlenwerk_runtime/src/music/soundtrack.v2m`.

Be sure to include `Dsound.lib` and `libv2.lib` in *Additional Dependencies* in Visual Studio.

Define `SYNTH_V2` preprocessor-define in `WindowsBackend.cpp`.

## Usage
### Debugging

To make use of platform independend debugging functions, define the preprocessor-define `_DEBUG`. Then you can use the macro `RT_DEBUG(str)` to print the c-string `str` to standard out/error. You must wrap all calls to `RT_DEBUG` in an `#ifdef _DEBUG`. To output the value of a non-cstring variable `myvar`, convert it:

	#ifdef _DEBUG
		RT_DEBUG((std::to_string(myvar) + "\n").c_str());
	#endif


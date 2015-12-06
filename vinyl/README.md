# vinyl -- Strahlenwerk Runtime

## Building

`vinyl` uses the [Premake](https://premake.github.io) meta build system to generate build system files for different platforms. Currently Premake 5.0 (alpha) in the latest development snapshot is used. A Linux Makefile and a Visual Studio 2013 Solution are already generated for your convenience.

Currently there are two "Configurations": `Release` and `Debug`. `Release` builds a small-as-possible binary without symbols, debug output and does not link against the standard library. This is suitable for entry in compos. `Debug` has all the fancy stuff like (OpenGL-) debug output. On Top of that, there are three supported audio sources (formally synthesizers) ("Platforms"): `4klang` (Windows & Linux), `V2` (Windows only), and `vorbis` (Windows & Linux). Premake generates projects that allow you to build any combination of those.

### Linux
Get the latest Premake 5.0 development snapshot for your distribution. On Arch this is [premake-git](https://aur.archlinux.org/packages/premake-git/) from the AUR. You also might need the [NASM](http://nasm.us) assembler. Install it from your distribution. On Arch this is [extra/nasm](https://www.archlinux.org/packages/extra/x86_64/nasm/).

Now run

    premake5 gmake

to generate a `Makefile` in `rtificial/vinyl/Builds/gmake/`. Unfortunately this is broken and you have to use the "project" specific makefile under `rtificial/vinyl/Builds/gmake/vinyl.make`.

    cd Builds/gmake/
    make -f vinyl.make

This will build the default configuration. As only 4klang is supported as synth on Linux, you can choose between `debug_4klang` and `release_4klang` configurations with the `config` option

    make -f vinyl.make config=release_4klang

For ease of use, you can simply use the `rtificial/vinyl/build_linux.sh` script to build all configurations at once.


### Windows
Follow the installation manual for Visual Studio in the [Strahlenwerk ReadMe.md](../strahlenwerk/ReadMe.md).

Download the latest Premake 5.0 development [alpha builds](https://premake.github.io/download.html#v5), extract and put it somewhere in your `%PATH%`. I recommend creating `%HOMEPATH%\bin`, putting `premake5.exe` there and [adding](http://www.computerhope.com/issues/ch000549.htm) it to your `%PATH`.

Now use a shell to run

    premake5.exe vsXXXX

for whatever Visual Studio version you are using. Open the "Solution" `rtificial/vinyl/Builds/vsXXXX/Demo.sln` in Visual Studio. You can now simply use the *Configuration Manager* and *Batch Build Dialogue* to choose what configuration and synth you want to use. 

It may be the case that you need a `nasm.exe` executable. It is used for include binary resources in viny. Download a standalone version (not the installer) from [nasm.us](http://nasm.us/) and extract it. Now copy the executable `nasm/nasm.exe` to `rtificial/vinyl/Builds/vsXXXX/nasm.exe`.


## Usage

### Audio Sources
#### 4klang
On Windows place the 4klang header into `rtificial/vinyl/Source/music/4klang.windows.h` and the `.obj` in `rtificial/vinyl/Source/music/4klang.windows.lib`.

On Linux place the 4klang export into `rtificial/vinyl/Source/music/4klang.linux.h` and `rtificial/vinyl/Source/music/4klang.linux.o`.

#### V2
Download `libv2.lib` from [pouet.net](http://www.pouet.net/prod.php?which=15073) and place it int `rtificial\vinyl\Lib\lib\libv2.lib`.

Place the V2 export into `rtificial/vinyl/Source/music/soundtrack.v2m` and set the `BPM` in `rtificial/vinyl/Spource/music/bpm.h`.

#### vorbis
Place the Ogg Vorbis file into `rtificial/vinyl/Source/music/soundtrack.ogg` and set the `BPM` in `rtificial/vinyl/Spource/music/bpm.h`. Note that at the current state it is not possible to decode the file in a thread parallel to compiling shaders on windows, this might lengthen the loading time a bit.


### Debugging

To make use of platform independent debugging functions, define the preprocessor-define `_DEBUG`. Then you can use the macro `RT_DEBUG(str)` to print the c-string `str` to standard out/error. You must wrap all calls to `RT_DEBUG` in an `#ifdef _DEBUG`. To output the value of a non-cstring variable `myvar`, convert it:

	#ifdef _DEBUG
		RT_DEBUG((std::to_string(myvar)``).c_str());
	#endif

A newline character `\n` is automatically appended to the string.

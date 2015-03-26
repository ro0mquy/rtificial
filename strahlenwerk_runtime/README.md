# Strahlenwerk Runtime

## Building
### Linux
*it just doesn't work*

### Windows
Follow the installation manual for Visual Studio in the [Strahlenwerk ReadMe.md](../strahlenwerk/ReadMe.md).

### Using the 4klang synth
Place the 4klang export into `rtificial/strahlenwerk_runtime/src/music/4klang.windows.h` and `rtificial/strahlenwerk_runtime/src/music/4klang.windows.obj`.

Be sure to include `winmm.lib` and `4klang.windows.obj` in *Properties* > *Configuration Properties* >*Linker* > *Input* >*Additional Dependencies* in Visual Studio.

#### Using the V2 synth
Download the `libv2` from [pouet](http://www.pouet.net/prod.php?which=15073) and unpack it. Copy `libv2/libv2.h`, `libv2/v2mplayer.h`, and `libv2/v2mplayer.cpp` to `rtificial/strahlenwerk_runtime/Lib/include/` and `libv2/libv2.lib` to `rtificial/strahlenwerk_runtime/Lib/lib/`.

It may be the cases that you need a `nasm.exe` executable. Download a standalone version (not the installer) from [nasm.us](http://nasm.us/) and extract it. Now copy the executable `nasm/nasm.exe` to `rtificial/strahlenwerk_runtime/VisualStudio2013/nasm.exe`

Place the V2 export into `rtificial/strahlenwerk_runtime/src/music/soundtrack.v2m`.

Be sure to include `Dsound.lib` and `libv2.lib` in *Additional Dependencies* in Visual Studio.

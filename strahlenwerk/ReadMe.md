﻿# Strahlenwerk
*ist gerade noch etwas doof aber ich arbeite dran*

## Controls
### Movement
* `WASD` move around
* `EC` move up/down
* `IJKL` look around
* `UO` rotate
* Hold down `[Alt]` or click into OpenGlComponent to look around with the mouse
* `[Scroll]|=|-` change movement speed (with mouse in OpenGlComponent or `[Alt]` down)
* `[Ctrl]+=|-` change rotation speed
* `[Ctrl]+[Scroll]` change focal length (with mouse in OpenGlComponent or `[Alt]` down)

### Timeline
* `[Scroll]` scroll timeline vertically
* `[Shift]+[Scroll]` scroll timeline horizontically
* `[Ctrl]+[Scroll]` zoom timeline (zoom in to mouse position, zoom out of screen center)
* `[Click]` set time position without changing camera
* `[Shift]+[Click]` set time position and change camera
* `[Space]` Play/pause without animating the camera
* `[Shift]+[Space]` Play/pause with animated camera
* `P` Place a Keyframe for the camera uniforms at the current position
* `B` Reset Camera Rotation
* `[Ctrl]+B` Reset Camera Position
* `[Ctrl]+S` save all timeline data to `timeline.xml`
* `[Rightclick]` Select Scene, Sequence or Keyframe
* ```|[Del]|[Backspace]`` Delete current selection
* `8` toggle looping of current selection
* `T` split selected sequences at current time or selected keyframe
* `[Ctrl]+T` merge selected sequences

#### Scenes
* `[Ctrl]+[Drag]` create new scene or move already existing
* `[Ctrl]+[Shift}+[Drag]` copy scene
* `[Ctrl]+[Rightclick]` select shader source name
* `[Ctrl]+[Middleclick]` delete scene

#### Sequences
* `[Ctrl]+[Drag]` create new sequence or move already existing sequence or keyframe
* `[Ctrl]+[Shift}+[Drag]` copy sequence or keyframe
* `[Ctrl]+[Click]` create keyframe when inside a sequence
* `[Ctrl]+[Rightclick]` select interpolation method
* `[Ctrl]+[Middleclick]` delete sequence or keyframe

### General
* `[Ctrl]+Q` quit
* `[Ctrl]+[Shift]+R` reload timeline
* `[Ctrl]+R` reload environment maps
* `[Ctrl]+O` open a project directory
* `F` Toggle fullscreen
* `G` Toggle Rule of Thirds grid
* `[Ctrl]+G` Toggle OpenGL window overlays
* `[Ctrl]+W` Toggle audio waveform preview
* `[Ctrl]+D` Make demo
* `[Ctrl]+M` Toggle audio mute
* `F12` Save Screenshot (to `~/Dropbox/rtificial/Screenshots/<ProjectName>`, configrable via `strahlenwerk.settings` key `screenshot_dir`)

(Shader is also reloaded on file change)

### Camera
* By default `camera_position` and `camera_rotation` uniforms are used.
* Crane camera control can be toggled by `camera_crane_active`.
* Crane head position is given in spherical coordinates relative to base.
* All angles are measured in units of Tau.
* Theta is angle between up-axis (y-axis) and head.
* Phi is angle in ground plane (xz-plane).
* Target tracking can be toggled by `camera_tracking_active`.
* Camera will almost always ensure that the target is at `camera_tracking_screen_pos` (range 0-1, origin at bottom left)
* Roll gives rotation angle of the camera around the viewing axis in units of Tau.

## Building and Installing
### Linux
#### Install Juce
You have to `git clone --depth 1 git://github.com/julianstorer/JUCE.git` into any folder you want. Create then a symlink called `juce_modules` in `rtificial/strahlenwerk` to the modules folder of Juce.
Example:
    cd rtificial/strahlenwerk && \
        git clone --depth 1 git://github.com/julianstorer/JUCE.git ~/Coding/JUCE && \
        ln -s ~/Coding/JUCE/modules juce_modules

#### Build dependencies
* [efsw](https://bitbucket.org/SpartanJ/efsw) -- cross-platform file system watcher and notifier
* [glm](http://glm.g-truc.net/) -- mathematics library for graphics software based on glsl

#### Install external libraries
Go to `Lib` and execute `./build.sh` to install efsw. You probably need mercurial and cmake.
Install all other external libraries with your favorite packet manager.

#### Building
Go to `Builds/Linux` and execute `make -j 5`.

#### Running
Just run `Builds/Linux/build/strahlenwerk` and be happy with this awesome looking piece of software.


### Windows
#### Install Visual Studio
You need *Microsoft® Visual Studio® Express 2013 with Update 3 for Windows® **Desktop*** (because of some C++11 features). [Download](http://www.visualstudio.com/downloads/download-visual-studio-vs#d-express-windows-desktop) and install it. You probably need a *Live* Account and 5GB space on EVERY DISK. I don't even...

#### Install JUCE
Use Git Shell to clone JUCE in any folder you want.

    git clone --depth 1 git://github.com/julianstorer/JUCE.git

Now use `cmd.exe` (not PowerShell) and go to `rtificial\strahlenwerk`. Then run

    mklink /D juce_modules path\to\JUCE\modules

where `path\to\JUCE\modules` is the path to the `JUCE\modules` directory. This will create a symbolic link.

#### Installing build dependencies
Download `glm` from [its home page](http://glm.g-truc.net) and unpack it. Move `glm-0.9.x.y\glm\glm` to `rtificial\strahlenwerk\Lib\include\glm`. You don't need the rest.


Download `premake` from [its home page](http://industriousone.com/premake/download) and unpack it. Now download the [`efsw` zip-ball](https://bitbucket.org/SpartanJ/efsw/downloads) and unpack it. Now create a Visual Studio project by running the following in `cmd.exe`:

    path\to\premake4.exe vs2010

Open the project `.\make\Windows\efsw-static.vcxproj` in Visual Studio and compile it 
(Note: if you get LNK2038-Errors you might have to manually change the compileroption /MDd to /MTd). 
Now move `.\lib\efsw-debug.lib` to `rtificial\strahlenwerk\Lib\lib\efsw.obj` and the folder `.\include\efsw` to `rtificial\strahlenwerk\Lib\include\efsw`. 
Copy `.\lib\efsw-debug.dll` to `rtificial\strahlenwerk\Builds\VisualStudio2013\Debug\efsw-debug.dll` or your desired VS version and build config. You don't need the rest.

#### Building and running
Open `rtificial\strahlenwerk\Builds\VisualStudio2013\strahlenwerk.vcxproj` in Visual Studio and hit `F7` to build, `F5` to build-and-run with the debugger and `Ctrl+F5` to build-and-run without the debugger. Hopefully nothing breaks, good luck with that! And you don't even need to reboot your computer twice and wait for a myriad of "finishing updates" to complete. It just runs!


## Scene directory contents
### Directory structure
*(todo)*

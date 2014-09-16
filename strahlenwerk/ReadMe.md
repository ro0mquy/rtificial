# Strahlenwerk
*ist gerade noch etwas doof aber ich arbeite dran*

## Controls
### Movement
* `WASD` move around
* `EC` move up/down
* `IJKL` look around
* `UO` rotate

### Timeline
* `[Scroll]` scroll timeline vertically
* `[Shift]+[Scroll]` scroll timeline horizontically
* `[Ctrl]+[Scroll]` zoom timeline (zoom in to mouse position, zoom out of screen center)
* `[Click]` set time position
* `[Ctrl]+S` save all timeline data to `timeline.xml`

#### Scenes
* `[Ctrl]+[Drag]` create new scene or move already existing
* `[Ctrl]+[Doubleclick]` edit shader source name
* `[Ctrl]+[Middleclick]` delete scene

#### Sequences
* `[Ctrl]+[Drag]` create new sequence or move already existing sequence or keyframe
* `[Ctrl]+[Click]` create keyframe when inside a sequence
* `[Ctrl]+[Middleclick]` delete sequence or keyframe

### General
* `[Esc]` quit
* `[F5]` reload shaders
* `[Ctrl]+O` open a project directory

(Shader is also reloaded on file change)

## Building and Installing
### Install Juce
You have to `git clone --depth 1 git://github.com/julianstorer/JUCE.git` into any folder you want. Create then a symlink called `juce_modules` in `rtificial/strahlenwerk` to the modules folder of Juce.
Example:
    cd rtificial/strahlenwerk && \
        git clone --depth 1 git://github.com/julianstorer/JUCE.git ~/Coding/JUCE && \
        ln -s ~/Coding/JUCE/modules juce_modules

### Build dependencies
* [efsw](https://bitbucket.org/SpartanJ/efsw) -- cross-platform file system watcher and notifier
* [glm](http://glm.g-truc.net/) -- mathematics library for graphics software based on glsl

### Install external libraries
Go to `Lib` and execute `./build.sh` to install efsw. You probably need mercurial and cmake.
Install all other external libraries with your favorite packet manager.

### Building
Go to `Builds/Linux` and execute `make -j 5`.

### Running
Just run `Builds/Linux/build/strahlenwerk` and be happy with this awesome looking piece of software.

## Scene directory contents
### Directory structure
*(todo)*

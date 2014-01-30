# blinkizeuch™
*runnin' all your favorite shaders with ease*

## Controls

### Movement

* `WASD` move around
* `QE` move up/down
* `IJKL` look around
* `UO` rotate

### Timeline

* `P` place keyframe
* `R` delete (remove) keyframe nearest to the mouse
* `[Scroll]` zoom timeline (zoom in to mouse position, zoom out of screen center)
* `[Ctrl]+[Scroll]` scroll timeline horizontically
* `[Click]` jump to position staying ad current camera position
* `[Ctrl]+[Click]` jump to position moving to interpolated camera position
* `[Space]` start animated flight through keyframes
* `H` toggle timeline
* `[F8]` save timeline (keyframes) to `timeline.json`

### General
* `F` toggle full screen
* `[Esc]` quit
* `[F5]` reload shader
* `[F7]` save tweakables to `config.json`

(Shader is also reloaded on file change and on receiving `SIGHUP`)

## Building and Installing

### Build dependencies

You need the following dependencies

* [SDL](http://www.libsdl.org/) version 1.2.15, not 2.x! -- Simple Direct Media Layer
* [AntTweakBar](http://anttweakbar.sourceforge.net/doc/tools:anttweakbar?sb=tools) version 1.14 -- A library that adds an easy GUI into OpenGL applications to interactively tweak them on-screen
* [DevIL](http://openil.sourceforge.net/) version 1.7.8 -- A full featured cross-platform image library
* [Jansson](http://www.digip.org/jansson/) minimum Version: 2.5 -- C library for encoding, decoding and manipulating JSON data
* libzeuch bleeding edge version -- internal library for shader loading, vector math and other abstraction

### Dependency Installation

#### Gentoo

    USE="X audio opengl png sdl" \
    emerge \
        '<media-libs/libsdl-2.0' \
        '~media-libs/anttweakbar-1.14' \
        '>=dev-libs/jansson-2.5' \
        '~media-libs/devil-1.7.8'

#### OS X

    brew install \
        sdl \
        anttweakbar \
        devil \
        jansson

### Building

After you build and installed all dependencies, just run

    make

and you should be happily provided with a `blinkizeuch` binary. `make clean` should clean up the mess you made with all that object files, you dirty little girl.

### Running

Usage:

    blinkizeuch scenedir/

Where `scenedir/` points to a scene directory.

## Scene directory contents

### Directory structure

A scene directory is a directory where a scene and all its assets are stored. You'd guessed that?! Right now, most of them are kept in the `scenes` directory (what a surprise!). It is made up of the following files:

    % tree scenes/template
    scenes/template
    ├── config.json
    ├── scene.glsl
    └── timeline.json

You can easily generate the directory structure for a new scene by running

    ./create_project.sh $projectname

### scene.glsl

This is the fragment shader file. It will be loaded for display in the program. You are provided with the following uniforms for your convenience:

    uniform vec3 color_foo1;
    uniform vec3 color_foo2;
    uniform float foo1;
    uniform float foo2;

Which represent tweakables to be tweaked in the program. For more information, refer to the libblink documentation (bwahaha documentation).

### config.json

This is a configuration file for variables and textures to be used in the shader program. The JSON consists of a root object with two keys:

`tweakables` contains an array of tweakable-objects. These have the following attributes:

* `name`: string containing the name of the variable
* `uniform`: string containing the name of the uniform variable as accessed in the shader. It better be all ascii without spaces or god knows what might happen
* `type`: string specifying the variable type as a string. May be either `bool`, `float` or `color`
* `varparam` (optional) a string which contains parameters directly passed to AntTweakBar. Refer to [their documentation](http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:varparamsyntax).
* `defaul`: (optional) specifies the default values to be loaded. It will also be saved by blinkzeuch once set. Please use the correct type. `color`s are represented by an array of three floats between 0 and 1 in RGB format.

`textures` contains an array of texture-objects. These have the following attributes:

* `path`: the path **relative to the blinkizeuch working directory** to the texture file in png format.
* `uniform`: the name of the uniform variable to be accessed in the shader.

An example:

    {
            "tweakables": [
                    {
                            "name": "Glow color",
                            "uniform": "glow_color",
                            "type": "color",
                            "default": [1.0, 0.5, 0.0],
                            "varparam": "colormode=hls"
                    }
            ],
            "textures": [
                    {
                            "path": "test.png",
                            "uniform": "tex1"
                    }
            ]
    }

### timeline.json

This is a configuration -- or better: storage -- file for the timeline. The JSON consists of a root object with just one key (for now): `keyframes`. This holds an array of keyframe-objects. The keyframes are flown through during a camera movement animation. A keyframe-object stores the following attributes:

* `time`: integer time stamp at which the keyframe shall be reached.
* `position`: an array of three floats containing the cartesian coordinates `x`, `y` and `z` of the camera posiotion.
* `rotation`: an array of four floats containing some weirdly mathematical representation of strange 4-dimensional rotation information. Super spacey! (FYI: the last digit is the real part)

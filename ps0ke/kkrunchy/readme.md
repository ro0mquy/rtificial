# kkrunchy
## kkrunchy binaries (and other exe packers) -- gotta collect them all

This is our collection of different binary versions of the exe packer [kkrunchy](http://www.farbrausch.de/~fg/kkrunchy/) and BeroExePacker (http://www.rosseaux.net/page40_BeRoEXEPacker.html). Keeping different versions is useful when one version breaks your binary by compressing it.

## `kkrunchy.bat`
This batch script runs all our exe packer versions against `vinyl`s `Debug` and `Release` executables of one build configuration so that you can find the one that works and compresses best.

Usage: `kkrunchy.bat <config>` where `config` is a build configuration, which might be one of `4klang`, `V2`, `vorbis` and more. See `rtificial/vinyl/README.md` for all available configurations.

## Versions

### `bep.exe`
BeRoEXEPacker - Version 1.00.2012.04.29, downloaded from bero's website in 2015

### `kkrnuchy_021.exe`
This is a version we got from cupe on deadline14.

### `kkrunchy_023a.exe`
Was downloaded from the website in 2014.

### `kkrunchy_023a2_k7.exe`
Was downloaded from the website in 2014.

### `kkrnuchy_023a4_asm07.exe`
This is a version we got from cupe on deadline14.

### Conspiracy
Conspiracy gave us two modified kkrunchy versions at revision16. The binaries reside in `conspiracy/Tools` and they even provided (probably broken) sourcecode in `conspiracy/Utils`. They supposedly throw an error message when the compressed binary tries to load a missing DLL instead of just dying silently.

* `kkrunchy_023_2014_importhack.exe`
* `rekkrunchy.exe`

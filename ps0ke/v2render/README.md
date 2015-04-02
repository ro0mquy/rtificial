# v2render
Use this to render `.v2m` files to a file.

You might need to put a `nasm.exe` into this directory. See [`strahlenwerk_runtime/README.md`](../../strahlenwerk_runtime/README.md) for more information.

Place the soundtrack as `soundtrack.v2m` into this directory, compile the project (only Debug configuration will work) and run it. Output will be written to `audio.out` in a raw format. Use Audacity via *File* > *Import* > *Raw Data* to open the file. Use the following import settings if they are not recognized automatically:

|Codec:|32 bit float|
|Byte-Order:|Little Endian|
|Channels:|2 Channels (stereo)|
|0-Byte-Offset:|0 Bytes|
|Datenmenge:|100%|
|Samplefrequency: 44100 Hz|

You can now export it as a normal audio file for playback in a format of your choice.

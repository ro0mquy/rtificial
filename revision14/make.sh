#!/bin/sh

gcc -std=c99 -lm -Os -s -fomit-frame-pointer -m32 -o main main.c 4klang.o libzeuch/libzeuch.a -I. $(sdl-config --cflags --libs) $(pkg-config --cflags --libs glew) -Wall -lm
xz -f main
cat header main.xz > rmk_hier_klicken
chmod +x rmk_hier_klicken

#!/bin/sh

gcc -std=c99 -lm -Os -s -fomit-frame-pointer -m32 -o main main.c 4klang.o libzeuch/libzeuch.a -I. $(sdl-config-32 --cflags --libs) $(pkg-config --cflags --libs glew) -Wall


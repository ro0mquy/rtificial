#!/bin/sh

gcc -Os -m32 -o main main.c 4klang.o libzeuch/libzeuch.a -I. $(sdl-config-32 --cflags --libs) $(pkg-config --cflags --libs glew) -Wall


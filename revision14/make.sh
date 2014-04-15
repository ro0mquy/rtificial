#!/bin/sh

gcc -o main main.c libzeuch/libzeuch.a -I. $(sdl-config --cflags --libs) $(pkg-config --cflags --libs glew)

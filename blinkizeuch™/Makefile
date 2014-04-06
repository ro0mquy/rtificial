LIBZEUCH_ROOT=../
LIBZEUCH=$(LIBZEUCH_ROOT)libzeuch/libzeuch.a

CFLAGS=-Os -std=c99 -Wall -Wextra $(shell sdl-config --cflags) -I$(LIBZEUCH_ROOT) $(shell pkg-config --cflags jansson IL) $(shell freetype-config --cflags)
LDLIBS=-lGLEW $(shell sdl-config --libs) $(LIBZEUCH) -lAntTweakBar $(shell pkg-config --libs jansson IL) $(shell freetype-config --libs)
LINUX_LDLIBS=-lGL -lm
DARWIN_LDLIBS=-framework Cocoa -framework OpenGL

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    LDLIBS+=$(LINUX_LDLIBS)
else ifeq ($(UNAME), Darwin)
    LDLIBS+=$(DARWIN_LDLIBS)
endif

OBJECTS=\
	main.o\
	camera.o\
	flight.o\
	font.o\
	scene.o\
	tweakable.o\
	texture.o\
	util.o\
	timeline.o\
	window.o\
	music.o\

HEADER=\
	config.h\
	camera.h\
	vertex.h\
	flight.h\
	font.h\
	scene.h\
	tweakable.h\
	texture.h\
	util.h\
	timeline.h\
	timeline_vertex.h\
	timeline_fragment.h\
	window.h\
	music.h\

.PHONY: all
all: blinkizeuch

blinkizeuch: $(OBJECTS) $(LIBZEUCH)
	$(CXX) $(LDFLAGS) -o blinkizeuch $(OBJECTS) $(LDLIBS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o blinkizeuch

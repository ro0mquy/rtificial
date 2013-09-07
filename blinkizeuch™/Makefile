LIBZEUCH_ROOT=../
LIBZEUCH=$(LIBZEUCH_ROOT)libzeuch/libzeuch.a

CFLAGS=-Os -std=c99 -Wall -Wextra $(shell sdl-config --cflags) -I$(LIBZEUCH_ROOT) $(shell pkg-config --cflags jansson)
LDLIBS=-lGLEW $(shell sdl-config --libs) $(LIBZEUCH) -lAntTweakBar $(shell pkg-config --libs jansson)
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
	tweak_loader.o\

HEADER=\
	config.h\
	camera.h\
	vertex.h\
	flight.h\
	tweak_loader.h\

.PHONY: all
all: blinkizeuch

blinkizeuch: $(OBJECTS) $(LIBZEUCH)
	$(CXX) $(LDFLAGS) -o blinkizeuch $(OBJECTS) $(LDLIBS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o blinkizeuch

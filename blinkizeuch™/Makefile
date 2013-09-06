LIBZEUCH_ROOT=../
LIBZEUCH=$(LIBZEUCH_ROOT)libzeuch/libzeuch.a

CFLAGS=-Os -std=c99 -Wall -Wextra $(shell sdl-config --cflags) -I$(LIBZEUCH_ROOT)
LDLIBS=-lGL -lGLEW -lm $(shell sdl-config --libs) -lAntTweakBar $(LIBZEUCH)
# Mac OS alternate cmdline link options
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDLIBS=-lGLEW -framework Cocoa -framework OpenGL $(shell sdl-config --libs) $(LIBZEUCH)
endif

OBJECTS=\
	main.o\
	camera.o\
	flight.o\

HEADER=\
	config.h\
	camera.h\
	vertex.h\
	flight.h\

.PHONY: all
all: blinkizeuch

blinkizeuch: $(OBJECTS) $(LIBZEUCH)
	$(CXX) $(LDFLAGS) -o blinkizeuch $(OBJECTS) $(LDLIBS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o blinkizeuch

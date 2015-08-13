CFLAGS=-Os -std=c99 -Wall -Wextra $(shell sdl-config --cflags)
LDLIBS=-lGL -lGLEW $(shell sdl-config --libs)
# Mac OS alternate cmdline link options
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDLIBS=-lGLEW -framework Cocoa -framework OpenGL $(shell sdl-config --libs)
endif

OBJECTS=\
	main.o

HEADER=\
	config.h

LIBZEUCH=../../libzeuch/libzeuch.a

.PHONY: all
all: partikel

partikel: $(OBJECTS) $(LIBZEUCH)
	$(CC) $(LDFLAGS) -o partikel $(OBJECTS) $(LIBZEUCH) $(LDLIBS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o partikel

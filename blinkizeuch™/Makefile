CFLAGS=-Os -std=c99 -Wall -Wextra $(shell sdl-config --cflags)
LDLIBS=-lGL -lGLEW -lm $(shell sdl-config --libs)
# Mac OS alternate cmdline link options
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDLIBS=-lGLEW -framework Cocoa -framework OpenGL $(shell sdl-config --libs)
endif

OBJECTS=\
	main.o

HEADER=\
	config.h

LIBZEUCH=../libzeuch/libzeuch.a

.PHONY: all
all: blinkizeuch

blinkizeuch: $(OBJECTS) $(LIBZEUCH)
	$(CC) $(LDFLAGS) -o blinkizeuch $(OBJECTS) $(LIBZEUCH) $(LDLIBS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o blinkizeuch

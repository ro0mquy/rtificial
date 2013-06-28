CFLAGS=-Os -std=c99 -Wall -Wextra $(shell sdl-config --cflags)
LDLIBS=-lGL -lGLEW $(shell sdl-config --libs)

OBJECTS=\
	main.o\
	shader.o

.PHONY: all
all: blinkizeuch

blinkizeuch: $(OBJECTS)
	$(CC) $(LDFLAGS) -o blinkizeuch $(OBJECTS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *.o blinkizeuch

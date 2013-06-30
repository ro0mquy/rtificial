CFLAGS=-Os -std=c99 -Wall -Wextra

OBJECTS=\
	shader.o

HEADER=\
	gl.h\
	shader.h

.PHONY: all
all: $(OBJECTS)
	ar rcs libzeuch.a $(OBJECTS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o libzeuch.a

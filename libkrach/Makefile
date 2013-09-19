CFLAGS=-Os -std=c99 -Wall -Wextra

OBJECTS=\
	synth.o\
	osc.o\
	envelope.o\

HEADER=\
	synth.h\
	osc.h\
	envelope.h\

.PHONY: all
all: $(OBJECTS)
	ar rcs libkrach.a $(OBJECTS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o libkrach.a

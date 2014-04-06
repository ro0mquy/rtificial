generate: 4klang.o 4klang.inh generate.c
	$(CC) -m32 -o generate generate.c 4klang.o

4klang.o: 4klang.asm 4klang.inc
	yasm -f elf32 4klang.asm

clean:
	$(RM) 4klang.o generate

ASMC=nasm
CC=gcc


entrypoint.bin: entry.asm
	$(ASMC) $< -f bin -o $@

all: entrypoint.bin

clean:
	rm *.bin *.o *.out

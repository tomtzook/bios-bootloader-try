
entrypoint.bin: entry.asm
	nasm $< -f bin -o $@

all: entrypoint.bin

clean:
	rm *.bin *.o

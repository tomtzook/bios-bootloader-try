
entrypoint.bin: entry.asm
	nasm $< -f bin -o $@

main_entry.o: main_entry.asm
	nasm $< -f elf -o $@

main.o: main.c
	gcc -m32 -fno-pie -ffreestanding -c $< -o $@

main.bin: main_entry.o main.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

image.bin: entrypoint.bin main.bin
	cat $^ > $@


run: image.bin
	qemu-system-i386 -drive format=raw,file=$<

# -s S refer to gdb stub and initial break
# target remote localhost:1234
# break *0x07c00
run-debug: image.bin
	qemu-system-i386 -drive format=raw,file=$< -s -S

all: image.bin
clean:
	rm *.bin *.o

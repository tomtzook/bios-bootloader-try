
entrypoint.bin: entry.asm
	nasm $< -f bin -o $@

main_entry.o: main_entry.asm
	nasm $< -f elf -o $@

main.o: main.c
	gcc -m32 -fno-pie -g -ffreestanding -c $< -o $@

main.bin: main_entry.o main.o
	ld -m elf_i386 -no-pie -o $@ -Ttext 0x1000 $^ --oformat binary

main_debug.elf: main_entry.o main.o
	ld -m elf_i386 -no-pie -o $@ -Ttext 0x1000 $^

image.bin: entrypoint.bin main.bin
	cat $^ > $@

run: image.bin
	qemu-system-i386 -drive format=raw,file=$<

# -s S refer to gdb stub and initial break
# target remote localhost:1234
# break *0x07c00 for bootloader start
# break *0x1000 for second stage start
run-debug: image.bin main_debug.elf
	qemu-system-i386 -drive format=raw,file=$< -s -S &
	gdb -ex "set architecture i386" -ex "target remote localhost:1234" -ex "symbol-file main_debug.elf"

all: image.bin
clean:
	rm *.bin *.o *.elf

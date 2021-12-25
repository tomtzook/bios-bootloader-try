
entrypoint.bin: entry.asm
	nasm $< -f bin -o $(OUTDIR)/$@

main_entry.o: main_entry.asm
	nasm $< -f elf -o $(OUTDIR)/$@

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c lib/*.c mem/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h lib/*.h mem/*.h)
OBJS = ${C_SOURCES:.c=.o cpu/interrupt.o cpu/gdt_flush.o cpu/interrupt.o}

CC = i686-elf-gcc
LD = i686-elf-ld
DB = i686-elf-gdb
AS = i686-elf-as
EM = qemu-system-x86_64
EFLAGS = -monitor stdio -s
CFLAGS = -ffreestanding -O0 -nostdlib -fno-pie -fno-pic
all: eos.bin

run: iso
	qemu-system-i386 eos.iso -d int -D debug.log
iso: eos.bin
	cp eos.bin isodir/boot/
	grub-mkrescue -o eos.iso isodir

boot.o: boot/boot.asm
	$(AS) $< -o boot.o

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $^ -o $@

# # $(LD) -m elf_i386 -T boot/kernel.ld $^ -o $@
eos.bin: boot.o kernel/kernel.o ${OBJS}
	i686-elf-gcc -T boot/kernel.ld -o eos.bin -ffreestanding -O2 -nostdlib $^ -lgcc

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) -c $< -o $@

cpu/interrupt.o: cpu/interrupt.asm
	nasm -f elf  $< -o $@
	
cpu/gdt_flush.o: cpu/gdt_flush.asm
	nasm -f elf $< -o $@
cpu/idt_flush.o: cpu/idt_flush.asm
	nasm -f elf $< -o $@
%.o: %.asm obj
	nasm $< -f elf -o $

clean:
	rm *.iso -f
	rm *.bin -f
	rm *.o -f
	find . -name \*.o -type f -delete

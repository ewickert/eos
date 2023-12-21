C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
OBJS = ${C_SOURCES:.c=.o obj/cpu/interrupt.o}

CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -m32 -g -ffreestanding -fno-pie -O0 -nolibc

all: run

kernel.bin: obj/boot/kernel_entry.o ${OBJS}
	$(LD) -m elf_i386 -s -o $@ -Ttext 0x1000 $^ --oformat binary 

obj/boot/kernel_entry.o: boot/kernel_entry.asm obj_folder
	nasm $< -f elf -o $@

obj/kernel.o:kernel/kernel.c
	$(CC) ($CFLAGS) -c $^ -o $@ 

kernel.elf: obj/boot/kernel_entry.o ${OBJS}
	$(LD) -s -o $@ -Ttext 0x1000 $^

mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

obj/%.o: %.c ${HEADERS} obj_folder
	$(CC) $(CFLAGS) -c $< -o $@
	
obj/cpu/interrupt.o: cpu/interrupt.asm obj_folder
	nasm $< -f elf -o $@

obj/%.o: %.asm obj
	nasm $< -f elf -o $@

obj_folder:
	mkdir -p obj
	mkdir -p obj/boot
	mkdir -p obj/kernel
	mkdir -p obj/cpu
	mkdir -p obj/drivers

run: os-image.bin kernel.elf
	qemu-system-i386 -fda $< -monitor stdio -s

debug: os-image.bin kernel.elf
	qemu-system-i686 $< -s -S & gdb -ex "set architecture i386" -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
clean: 
	rm -rf obj
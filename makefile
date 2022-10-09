CXX=i686-elf-g++
LD=i686-elf-gcc
CXXFLAGS=-Iinc -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers
NASM=nasm
ASFLAGS=-felf32
SOURCES=$(shell find . -name '*.cpp' -not -path './boot/*')
LINK_SOURCES=$(shell find . -name '*.o')
CPP_FILES_OUT = $(SOURCES:.cpp=.o)
.PHONY: all as clean link iso clean_objects run

all: as $(CPP_FILES_OUT) link clean_objects iso run
bochs: as $(CPP_FILES_OUT) link clean_objects iso run-bochs
as:
	nasm -felf32 boot/loader.asm -o boot/boot.a
	nasm -felf32 'asm/paging_asm.asm' -o 'asm/paging_asm.o'
	nasm -felf32 'asm/gdt_asm.asm' -o 'asm/gdt_asm.o'
	nasm -felf32 'asm/idt_asm.asm' -o 'asm/idt_asm.o'

link:
	$(LD) -w -T 'linker.ld' -o 'kernel.bin' -ffreestanding -O2 -nostdlib boot/boot.a $(LINK_SOURCES) -lgcc
	strip kernel.bin

clean:
	-rm -Rf $(shell find . -name '*.o') $(shell find . -name '*.a') $(shell find . -name '*.bin') $(shell find . -name '*.iso') isodir

clean_objects:
	-rm -Rf $(shell find . -name '*.o') $(shell find . -name '*.a')

iso:
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot
	cp boot/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o NexusOS.iso isodir -V "NexusOS"

run:
	qemu-system-i386 -cdrom NexusOS.iso -m 512  -vga std -serial stdio
run-kvm:
	qemu-system-i386 -cdrom NexusOS.iso -m 512  -vga std -serial stdio -enable-kvm
run-monitor:
	qemu-system-i386 -cdrom NexusOS.iso -m 512  -vga std -monitor stdio -enable-kvm

run-bochs:
	bochs -q -f bochsrc.txt 

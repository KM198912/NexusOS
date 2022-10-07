CXX=i686-elf-g++
LD=i686-elf-gcc
CXXFLAGS=-Iinc -m32 -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable -w -Wno-narrowing -Wno-sign-compare -Wno-type-limits -Wno-unused-parameter -Wno-missing-field-initializers
NASM=nasm
ASFLAGS=-felf32
SOURCES=$(shell find . -name '*.cpp')
LINK_SOURCES=$(shell find . -name '*.o')
CPP_FILES_OUT = $(SOURCES:.cpp=.o)
.PHONY: all as clean link iso clean_objects run

all: as $(CPP_FILES_OUT) link clean_objects iso run

as:
	as --32 'boot/boot.asm' -o 'boot/boot.a'

link:
	$(LD) -w -T 'linker.ld' -o 'kernel.bin' -ffreestanding -O2 -nostdlib boot/boot.a $(LINK_SOURCES) -m32
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
	qemu-system-i386 -cdrom NexusOS.iso -m 512 -vga std -serial stdio -enable-kvm

rm -rf build
mkdir -p build/arch
mkdir -p build/kernel
mkdir -p build/driver
mkdir -p build/util

export CFLAGS="-std=c++11 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Isrc"

nasm -felf32 src/arch/boot.s -o build/arch/boot.o
nasm -felf32 src/arch/helpers.s -o build/arch/helpers.o

i686-elf-g++ -c src/kernel/arch.cpp -o build/kernel/arch.o $CFLAGS
i686-elf-g++ -c src/kernel/entry.cpp -o build/kernel/entry.o $CFLAGS
i686-elf-g++ -c src/kernel/globals.cpp -o build/kernel/globals.o $CFLAGS
i686-elf-g++ -c src/kernel/paging.cpp -o build/kernel/paging.o $CFLAGS
i686-elf-g++ -c src/kernel/kstd.cpp -o build/kernel/kstd.o $CFLAGS
i686-elf-g++ -c src/kernel/frame_alloc.cpp -o build/kernel/frame_alloc.o $CFLAGS
i686-elf-g++ -c src/kernel/page_alloc.cpp -o build/kernel/page_alloc.o $CFLAGS


i686-elf-g++ -c src/driver/vga_terminal.cpp -o build/driver/vga_terminal.o $CFLAGS
i686-elf-g++ -c src/driver/pit.cpp -o build/driver/pit.o $CFLAGS
i686-elf-g++ -c src/driver/keyboard.cpp -o build/driver/keyboard.o $CFLAGS
i686-elf-g++ -c src/driver/serial.cpp -o build/driver/serial.o $CFLAGS

i686-elf-g++ -c src/util/StaticHeap.cpp -o build/util/StaticHeap.o $CFLAGS

i686-elf-ld -T src/linker.ld -o build/kernel.bin build/arch/*.o build/kernel/*.o build/driver/*.o build/util/*.o

rm -rf build
mkdir -p build/arch
mkdir -p build/kernel

export CFLAGS="-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Isrc"

nasm -felf32 src/arch/boot.s -o build/arch/boot.o
i686-elf-g++ -c src/kernel/entry.cpp -o build/kernel/entry.o $CFLAGS
i686-elf-g++ -c src/kernel/vga_terminal.cpp -o build/kernel/vga_terminal.o $CFLAGS
i686-elf-ld -T src/linker.ld -o build/kernel.bin build/arch/*.o build/kernel/*.o

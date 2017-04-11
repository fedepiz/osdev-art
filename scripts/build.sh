rm -rf build
mkdir -p build/arch
mkdir -p build/kernel
mkdir -p build/memory
mkdir -p build/driver
mkdir -p build/util
mkdir -p build/filesystem
mkdir -p build/kstd
mkdir -p build/kterm
mkdir -p build/tasks

export CFLAGS="-std=c++11 -nostdlib -nostartfiles -ffreestanding -O2 -fno-exceptions -fno-rtti -Isrc -lgcc"
export CWARNS="-Wall -Wextra"
nasm -felf32 src/arch/boot.s -o build/arch/boot.o
nasm -felf32 src/arch/helpers.s -o build/arch/helpers.o

i686-elf-g++ -c src/kernel/arch.cpp -o build/kernel/arch.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kernel/entry.cpp -o build/kernel/entry.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kernel/initialize.cpp -o build/kernel/initialize.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kernel/globals.cpp -o build/kernel/globals.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kernel/multiboot.cpp -o build/kernel/multiboot.o $CFLAGS $CWARNS

i686-elf-g++ -c src/memory/subsystem.cpp -o build/memory/subsystem.o $CFLAGS $CWARNS
i686-elf-g++ -c src/memory/MAllocator.cpp -o build/memory/MAllocator.o $CFLAGS $CWARNS
i686-elf-g++ -c src/memory/paging.cpp -o build/memory/paging.o $CFLAGS $CWARNS
i686-elf-g++ -c src/memory/frame_alloc.cpp -o build/memory/frame_alloc.o $CFLAGS $CWARNS
i686-elf-g++ -c src/memory/liballoc.cpp -o build/memory/liballoc.o $CFLAGS
i686-elf-g++ -c src/memory/liballoc_hooks.cpp -o build/memory/liballoc_hooks.o $CFLAGS $CWARNS
i686-elf-g++ -c src/memory/debug.cpp -o build/memory/debug.o $CFLAGS $CWARNS


i686-elf-g++ -c src/kstd/kstdio.cpp -o build/kstd/kstdio.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kstd/kstdlib.cpp -o build/kstd/kstdlib.o $CFLAGS $CWARNS 
i686-elf-g++ -c src/kstd/kcpp_support.cpp -o build/kstd/kcpp_support.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kstd/string.cpp -o build/kstd/string.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kstd/icxxabi.cpp -o build/kstd/icxxabi.o $CLFAGS $CWARNS

i686-elf-g++ -c src/driver/vga_terminal.cpp -o build/driver/vga_terminal.o $CFLAGS $CWARNS
i686-elf-g++ -c src/driver/pit.cpp -o build/driver/pit.o $CFLAGS $CWARNS
i686-elf-g++ -c src/driver/keyboard.cpp -o build/driver/keyboard.o $CFLAGS $CWARNS
i686-elf-g++ -c src/driver/serial.cpp -o build/driver/serial.o $CFLAGS $CWARNS

i686-elf-g++ -c src/util/heap_common.cpp -o build/util/heap_common.o $CFLAGS $CWARNS
i686-elf-g++ -c src/util/StaticHeap.cpp -o build/util/StaticHeap.o $CFLAGS $CWARNS
i686-elf-g++ -c src/util/DynamicHeap.cpp -o build/util/DynamicHeap.o $CFLAGS $CWARNS
i686-elf-g++ -c src/util/text.cpp -o build/util/text.o $CFLAGS $CWARNS

i686-elf-g++ -c src/tasks/tasks.cpp -o build/tasks/tasks.o $CFLAGS $CWARNS

i686-elf-g++ -c src/filesystem/VFS.cpp -o build/filesystem/VFS.o $CFLAGS $CWARNS
i686-elf-g++ -c src/filesystem/DataOps.cpp -o build/filesystem/DataOps.o $CFLAGS $CWARNS


i686-elf-g++ -c src/kterm/Shell.cpp -o build/kterm/Shell.o $CFLAGS $CWARNS
i686-elf-g++ -c src/kterm/Terminal.cpp -o build/kterm/Terminal.o $CFLAGS $CWARNS

i686-elf-gcc -T src/linker.ld -o build/kernel.bin build/**/*.o $CFLAGS $CWARNS
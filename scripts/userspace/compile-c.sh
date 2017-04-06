filename=$(basename "$1")
extension="${filename##*.}"
filename="${filename%.*}"
export CFLAGS="-m32 -nostdlib -ffreestanding"
i686-elf-gcc ${filename}.c -o ${filename}.o $CFLAGS
i686-elf-gcc ${filename}.c -o ${filename}.bin -Wl,--oformat=binary $CFLAGS 
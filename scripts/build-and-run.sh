rm osdev-art.iso
rm isodir/boot/kernel.bin
./scripts/build.sh
./scripts/make-iso.sh
./scripts/run.sh

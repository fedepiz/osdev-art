cp build/kernel.bin isodir/boot
genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o osdev-art.iso isodir

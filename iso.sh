#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/veni.kernel isodir/boot/veni.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "veni" {
	multiboot /boot/veni.kernel
}
EOF
grub-mkrescue -o veni.iso isodir

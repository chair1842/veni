#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/vkernel isodir/boot/vkernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "veni" {
	multiboot /boot/vkernel
}
EOF
grub-mkrescue -o veni.iso isodir

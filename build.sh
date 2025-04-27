#!/bin/bash

set -e
clear

BUILD_DIR=build
IMG=$BUILD_DIR/disk.img

echo "[CLEAN] Видаляю старий $BUILD_DIR..."
rm -rf $BUILD_DIR
echo "[CLEAN] Готово."

echo "[BUILD] Створюю папку $BUILD_DIR..."
mkdir -p $BUILD_DIR

echo "[BUILD] Збираю bootloader..."
nasm -f bin bootloader.asm -o $BUILD_DIR/bootloader.bin

echo "[BUILD] Збираю kernel_entry..."
nasm -f elf32 kernel_entry.asm -o $BUILD_DIR/kernel_entry.o

echo "[BUILD] Компілую kernel і команди..."
gcc -Wall -ffreestanding -fno-pic -fno-stack-protector -m32 -c kernel.c -o $BUILD_DIR/kernel.o
gcc -Wall -ffreestanding -fno-pic -fno-stack-protector -m32 -c cmds.c -o $BUILD_DIR/cmds.o

echo "[BUILD] Лінкую ядро через link.ld..."
ld -m elf_i386 -T link.ld -o $BUILD_DIR/kernel.bin $BUILD_DIR/kernel_entry.o $BUILD_DIR/kernel.o $BUILD_DIR/cmds.o

echo "[BUILD] Створюю пустий образ диска..."
dd if=/dev/zero of=$IMG bs=512 count=2880

echo "[BUILD] Записую bootloader у MBR..."
dd if=$BUILD_DIR/bootloader.bin of=$IMG conv=notrunc

echo "[BUILD] Записую kernel у сектори після bootloader..."
dd if=$BUILD_DIR/kernel.bin of=$IMG bs=512 seek=1 conv=notrunc

echo "[RUN] Запускаю в QEMU..."
qemu-system-i386 -fda build/disk.img

#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

# compiler and flags
CC=clang
CLFAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

# build the kernel
$CC $CLFAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c

$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel kernel.elf # load the kernel
#!/bin/bash -e

mkdir -p build
cd build
rm stm32-wifi-ir.elf || echo "Cannot remove. stm32-wifi-ir.elf not build?"
make stm32-wifi-ir.bin && \
make stm32-wifi-ir.list

#!/usr/bin/env bash
set -euo pipefail

APP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${APP_DIR}/build"
ELF="${BUILD_DIR}/stm32f407_uart_host"
BIN="${BUILD_DIR}/stm32f407_uart_host.bin"

if [ ! -f "${ELF}" ]; then
    echo "Missing ELF: ${ELF}"
    echo "Build first with:"
    echo "  cmake -S ${APP_DIR} -B ${BUILD_DIR}"
    echo "  cmake --build ${BUILD_DIR}"
    exit 1
fi

# Convert to raw binary for tools that prefer .bin
arm-none-eabi-objcopy -O binary "${ELF}" "${BIN}"

# Preferred: OpenOCD (works well with STM32 Discovery boards)
openocd \
  -f interface/stlink.cfg \
  -f target/stm32f4x.cfg \
  -c "program ${ELF} verify reset exit"

# If you prefer flashing the .bin instead of .elf:
# openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program ${BIN} verify reset exit"

# Alternative: st-flash (if installed)
# st-flash write "${BIN}" 0x8000000
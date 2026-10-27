cmake_minimum_required(VERSION 3.16)

project(stm32f407_uart_host C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_READELF arm-none-eabi-readelf)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(REPO_ROOT "${CMAKE_SOURCE_DIR}/../..")
set(STM32CUBE_ROOT "${REPO_ROOT}/third_party/stm32cube/STM32CubeF4")

add_compile_definitions(
    STM32F407xx
    USE_HAL_DRIVER
)

add_compile_options(
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
)

add_link_options(
    -T${CMAKE_SOURCE_DIR}/stm32f407vg.ld
    -Wl,-gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/stm32f407_uart_host.map
    -specs=nano.specs
    -specs=nosys.specs
)

# HAL sources
file(GLOB HAL_SRCS
    "${STM32CUBE_ROOT}/Drivers/STM32F4xx_HAL_Driver/Src/*.c"
)

# Startup + system
set(CMSIS_DEVICE_SRCS
    "${STM32CUBE_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c"
    "${STM32CUBE_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s"
)

# App source
set(APP_SRCS
    main.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${REPO_ROOT}/port/os/include
    ${REPO_ROOT}/port/os/stm32

    ${STM32CUBE_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${STM32CUBE_ROOT}/Drivers/CMSIS/Include
    ${STM32CUBE_ROOT}/Drivers/STM32F4xx_HAL_Driver/Inc
)

add_executable(stm32f407_uart_host
    ${APP_SRCS}
    ${HAL_SRCS}
    ${CMSIS_DEVICE_SRCS}
)

target_link_libraries(stm32f407_uart_host
    c
    gcc
    m
    nosys
)

add_custom_command(TARGET stm32f407_uart_host POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/stm32f407_uart_host ${CMAKE_BINARY_DIR}/stm32f407_uart_host.bin
    COMMAND ${CMAKE_SIZE} ${CMAKE_BINARY_DIR}/stm32f407_uart_host
)
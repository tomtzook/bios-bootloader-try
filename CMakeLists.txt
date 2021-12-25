cmake_minimum_required(VERSION 3.21)
project(bootloader C ASM_NASM)

set(CMAKE_LINKER /usr/bin/ld)
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

add_subdirectory(boot)

add_executable(bootloader main.c screen.c intrinsics.c)
target_compile_options(bootloader PRIVATE -m32 -fno-pie -g -ffreestanding)
target_link_options(bootloader PRIVATE
        -nostdlib -nolibc -nodefaultlibs
        -m elf_i386 -Ttext 0x1000 -no-pie
        ${CMAKE_CURRENT_BINARY_DIR}/boot/main_entry.o)
add_dependencies(bootloader bootloader_entry)

add_custom_target(bootloader_raw
        COMMAND objcopy -O binary bootloader bootloader.bin
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS bootloader
)

add_custom_target(image
        COMMAND cat ${CMAKE_CURRENT_BINARY_DIR}/boot/entrypoint.bin bootloader.bin > image.bin
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS bootloader_raw boot_entry
)

add_custom_target(run-qemu
        COMMAND qemu-system-i386 -drive format=raw,file=image.bin
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS image
)

# run debugger like so
# where ${CMAKE_CURRENT_BINARY_DIR} is the build folder
# gdb -ex "set architecture i386" -ex "target remote localhost:1234" -ex "symbol-file ${CMAKE_CURRENT_BINARY_DIR}/bootloader"
# break *0x07c00 for bootloader start
# break *0x1000 for second stage start
add_custom_target(run-qemu-debug
        COMMAND qemu-system-i386 -drive format=raw,file=image.bin -s -S
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS image
)
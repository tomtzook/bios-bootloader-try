
add_custom_target(bootloader_entry
        COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} main_entry.o OUTDIR=${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(boot_entry
        COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} entrypoint.bin OUTDIR=${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

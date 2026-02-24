add_executable(stm32f446ze_example)

target_sources(stm32f446ze_example
    PRIVATE
        stm32f446ze_example.cc
        ${CMAKE_SOURCE_DIR}/startup/startup_stm32f4.s
        #${CMAKE_SOURCE_DIR}/src/print/uart/st/printr_uart_stm32f4.cc
        ${CMAKE_SOURCE_DIR}/src/print/RTT/printr_rtt.cc
        ${CMAKE_SOURCE_DIR}/src/RTT/SEGGER_RTT.c

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        ${CMAKE_SOURCE_DIR}/
)

#TODO: is there any way to do this through the file_set?
target_include_directories(stm32f446ze_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_compile_definitions(stm32f446ze_example
    PRIVATE
        STM32F446xx
)

target_link_libraries(stm32f446ze_example
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:gcc>

    PRIVATE
        coco
        #TODO: Added for RTT, I should provide the implementation for the functions it needs.
        c

    PRIVATE
        options
        warnings
)

target_link_options(stm32f446ze_example
    PRIVATE
        LINKER:-L${CMAKE_SOURCE_DIR}/
        LINKER:-L${CMAKE_CURRENT_SOURCE_DIR}/
        LINKER:-T${CMAKE_CURRENT_SOURCE_DIR}/stm32f446ze.ld
)


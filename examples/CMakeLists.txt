add_executable(stm32f446ze_example)

target_sources(stm32f446ze_example
    PRIVATE
        stm32f446ze_example.cc
        ${CMAKE_SOURCE_DIR}/startup/startup_stm32f4.s

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_compile_definitions(stm32f446ze_example
    PRIVATE
        STM32F446xx
)

target_compile_options(stm32f446ze_example
    PRIVATE
        -fno-rtti
        -fno-exceptions
)

target_link_libraries(stm32f446ze_example
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:gcc>

    PRIVATE
        coco
)

target_link_options(stm32f446ze_example
    PRIVATE
        LINKER:-L${CMAKE_SOURCE_DIR}/
        LINKER:-L${CMAKE_CURRENT_SOURCE_DIR}/
        LINKER:-T${CMAKE_CURRENT_SOURCE_DIR}/stm32f446ze.ld
)


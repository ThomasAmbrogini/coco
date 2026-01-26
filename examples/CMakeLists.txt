add_executable(stm32f446ze_example)

target_sources(stm32f446ze_example
    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include

    PRIVATE
        stm32f446ze_example.cc
        ${CMAKE_SOURCE_DIR}/src/startup/startup_stm32f4.s
)

target_compile_definitions(stm32f446ze_example
    PRIVATE
        STM32F446xx
)

target_link_libraries(stm32f446ze_example
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:gcc>

    PRIVATE
        ProjectWarnings
        ProjectOptions
)

target_link_options(stm32f446ze_example
    PRIVATE
        LINKER:-T${CMAKE_CURRENT_SOURCE_DIR}/stm32f446ze.ld
)


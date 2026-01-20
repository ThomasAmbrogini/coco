add_executable(stm32f446ze_example)

target_sources(stm32f446ze_example
    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include

    PRIVATE
        stm32f446ze_example.cpp
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

#target_link_options(${PROJECT_NAME}
#    PRIVATE
#        LINKER:-T${CMAKE_CURRENT_SOURCE_DIR}/linkerscript/${COCO_STM32_MICRO}.ld
#)


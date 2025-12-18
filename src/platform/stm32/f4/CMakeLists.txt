target_sources(${PROJECT_NAME}
    PRIVATE
        mcu/startup_${ROS_STM32_MICRO}.s

    PRIVATE
        drivers/clk/stm32_clock.cpp
        system_interrupts.cpp
        vector_table.cpp
        stm32_main.cpp

    PRIVATE
        FILE_SET HEADERS
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ${MCU_MACRO}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:gcc>

    PRIVATE
        ProjectWarnings
        ProjectOptions
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        LINKER:-T${CMAKE_CURRENT_SOURCE_DIR}/linkerscript/${ROS_STM32_MICRO}.ld
)


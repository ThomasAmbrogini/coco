target_sources(${PROJECT_NAME}
    PRIVATE
       stm32_main.cpp
       vector_table.cpp
       system_interrupts.cpp
       drivers/clk/stm32_clock.cpp
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ${MCU_MACRO}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        #TODO: remove the current source dir when log does not have the include to uart.
        ${CMAKE_CURRENT_SOURCE_DIR}
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


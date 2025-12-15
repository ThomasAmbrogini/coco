add_executable(TemperatureTaker)

target_sources(TemperatureTaker
    PRIVATE
       stm32_main.cpp
       vector_table.cpp
       system_interrupts.cpp
       drivers/clk/stm32_clock.cpp
       drivers/ll/stm32f4xx_ll_gpio.c
       drivers/ll/stm32f4xx_ll_rcc.c
       drivers/ll/stm32f4xx_ll_tim.c
)

stm32_mcu_macro(${ROS_STM32_MICRO} MCU_MACRO)
if(NOT MCU_MACRO)
    message(FATAL_ERROR
        "The MCU_MACRO for ${ROS_STM32_MICRO} does not exist"
    )
endif()

target_compile_definitions(TemperatureTaker
    PRIVATE
        ${MCU_MACRO}
        #TODO: this has to be global
        $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(TemperatureTaker
    PRIVATE
        #TODO: remove the current source dir when log does not have the include to uart.
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TemperatureTaker
    PRIVATE
        src_common
        stm32_common

    PRIVATE
        #TODO: If I am also going to use clang, gcc can not be used here.
        gcc

    PRIVATE
        project_warnings
)

target_link_options(TemperatureTaker
    PRIVATE
        LINKER:-T${CMAKE_CURRENT_SOURCE_DIR}/linkerscript/${ROS_STM32_MICRO}.ld
)

target_compile_options(TemperatureTaker PRIVATE "-save-temps" "-g" "-O0")
target_compile_options(TemperatureTaker PRIVATE "-ffunction-sections" "-fdata-sections")
target_compile_options(TemperatureTaker PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics -fno-exceptions>
)

#TODO: this options have to be global (or at least based on the config type).
#TODO: try to use the LINKER: syntax to make agnostic to the compiler passing
#the options to the linker.
target_link_options(TemperatureTaker PRIVATE "LINKER:--gc-sections")


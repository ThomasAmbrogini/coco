add_executable(TemperatureTaker)

target_sources(TemperatureTaker
    PRIVATE
       stm32_main.cpp
       vector_table.cpp
       system_interrupts.cpp
       stm32_clock.cpp
       drivers/stm32f4xx_ll_gpio.c
       drivers/stm32f4xx_ll_rcc.c
       drivers/stm32f4xx_ll_tim.c
)

target_compile_definitions(TemperatureTaker
    PRIVATE
        ${ROS_STM32_MICRO}
        #TODO: this has to be global
        $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(TemperatureTaker
    PRIVATE
        #TODO: remove the current source dir when log does not have the include to uart.
        ${CMAKE_CURRENT_SOURCE_DIR}
        mcu
        drivers
        core
)

target_link_libraries(TemperatureTaker
    PRIVATE
        src_common
        stm32_common
)

target_compile_options(TemperatureTaker PRIVATE "-save-temps" "-g" "-O0")
target_compile_options(TemperatureTaker PRIVATE "-ffunction-sections" "-fdata-sections")
target_compile_options(TemperatureTaker PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics -fno-exceptions>
)

#TODO: this options have to be global (or at least based on the config type).
#TODO: try to use the LINKER: syntax to make agnostic to the compiler passing
#the options to the linker.
target_link_options(TemperatureTaker PRIVATE "-Wl,--gc-sections")
target_link_options(TemperatureTaker PUBLIC -T${CMAKE_CURRENT_SOURCE_DIR}/stm32f411.ld)
target_link_libraries(TemperatureTaker PRIVATE gcc)


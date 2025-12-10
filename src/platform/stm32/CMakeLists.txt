add_executable(TemperatureTaker)

target_sources(TemperatureTaker
               PRIVATE
                   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/vector_table.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/stm32_clock.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/core/gpio.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/core/timer.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/core/stm32f4xx_it.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/core/system_stm32f4xx.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx-hal-driver/Src/stm32f4xx_ll_gpio.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx-hal-driver/Src/stm32f4xx_ll_rcc.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx-hal-driver/Src/stm32f4xx_ll_tim.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx-hal-driver/Src/stm32f4xx_ll_utils.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx-hal-driver/Src/stm32f4xx_ll_exti.c
               )

#TODO: I do not like these defines.
set(STM_DEFINE_SYMS
    USE_FULL_LL_DRIVER
    USE_FULL_ASSERT=1U
    HSE_VALUE=25000000
    HSE_STARTUP_TIMEOUT=100
    LSE_STARTUP_TIMEOUT=5000
    LSE_VALUE=32768
    EXTERNAL_CLOCK_VALUE=12288000
    HSI_VALUE=16000000
    LSI_VALUE=32000
    VDD_VALUE=3300
    PREFETCH_ENABLE=1
    INSTRUCTION_CACHE_ENABLE=1
    DATA_CACHE_ENABLE=1
    STM32F411xE
    $<$<CONFIG:Debug>:DEBUG>
)

target_compile_definitions(TemperatureTaker PRIVATE ${STM_DEFINE_SYMS})

set(DEVICE_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx-hal-driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis-device-f4/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

target_include_directories(TemperatureTaker PRIVATE ${DEVICE_INCLUDE_DIRS})

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


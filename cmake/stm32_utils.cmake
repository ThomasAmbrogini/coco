# Utils function for the stm32
#
# Naming convenction used for stm32:
# e.g., STM32F411:
# F4: family
# 11: sub family
# F411: device line
#
# References:
# https://www.digikey.com/en/maker/tutorials/2020/understanding-stm32-naming-conventions
#
set(STM32_UTILS_REGEX_MCU_NAME "^stm32([fhlg][0-9])([0-9]+)")

function(is_valid_stm32_micro STM32_MICRO is_valid_var)
    string(TOLOWER ${STM32_MICRO} STM32_MICRO_NORM)
    string(REGEX MATCH ${STM32_UTILS_REGEX_MCU_NAME} ${is_valid_var} ${STM32_MICRO_NORM})
    return(PROPAGATE ${is_valid_var})
endfunction()

function(stm32_micro_extract_family STM32_MICRO family_var)
    string(TOLOWER ${STM32_MICRO} STM32_MICRO_NORM)
    string(REGEX MATCH ${STM32_UTILS_REGEX_MCU_NAME} REGEX_RES ${STM32_MICRO_NORM})
    set(${family_var} ${CMAKE_MATCH_1})
    return(PROPAGATE ${family_var})
endfunction()

function(stm32_micro_extract_sub_family STM32_MICRO sub_family_var)
    string(TOLOWER ${STM32_MICRO} STM32_MICRO_NORM)
    string(REGEX MATCH ${STM32_UTILS_REGEX_MCU_NAME} REGEX_RES ${STM32_MICRO_NORM})
    set(${sub_family_var} ${CMAKE_MATCH_2})
    return(PROPAGATE ${sub_family_var})
endfunction()

function(stm32_micro_extract_device_line STM32_MICRO device_line_var)
    string(TOLOWER ${STM32_MICRO} STM32_MICRO_NORM)
    string(REGEX MATCH "^stm32([fhlg][0-9]+)" REGEX_RES ${STM32_MICRO_NORM})
    set(${device_line_var} ${CMAKE_MATCH_1})
    return(PROPAGATE ${device_line_var})
endfunction()

function(stm32_mcu_macro STM32_MICRO mcu_macro_name)
    set(STM32_f411_MACRO "STM32F411xE")

    stm32_micro_extract_device_line(${STM32_MICRO} DEVICE_LINE)

    if (STM32_${DEVICE_LINE}_MACRO)
        set(${mcu_macro_name} ${STM32_${DEVICE_LINE}_MACRO})
        return(PROPAGATE ${mcu_macro_name})
    endif()
endfunction()


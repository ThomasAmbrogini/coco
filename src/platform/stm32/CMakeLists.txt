target_sources(${PROJECT_NAME}
    PRIVATE
        libc/libc_runtime.cpp

    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS
            cmsis
)

set(ROS_STM32_MICRO_VALUES "stm32f411")
list(JOIN ROS_STM32_MICRO_VALUES ", " ROS_STM32_MICRO_LIST)
set(ROS_STM32_MICRO "stm32f411" CACHE STRING "The stm target micro (Values: ${ROS_STM32_MICRO_VALUES})")

if (NOT ROS_STM32_MICRO IN_LIST ROS_STM32_MICRO_VALUES)
    list(JOIN ROS_STM32_MICRO_VALUES "\n  - " ROS_STM32_MICRO_FORMATTED_LIST)
    message(FATAL_ERROR
        "Inavlid ROS_STM32_MICRO: ${ROS_STM32_MICRO}\n"
        "Valid options are: \n"
        "  - ${ROS_STM32_MICRO_FORMATTED_LIST}"
    )
endif()

message(STATUS "Selected STM micro: ${ROS_STM32_MICRO}")

include(stm32_utils)

stm32_mcu_macro(${ROS_STM32_MICRO} MCU_MACRO)
if(NOT MCU_MACRO)
    message(FATAL_ERROR
        "The MCU_MACRO for ${ROS_STM32_MICRO} does not exist"
    )
endif()

stm32_micro_extract_family(${ROS_STM32_MICRO} STM32_MICRO_FAMILY)

add_subdirectory(${STM32_MICRO_FAMILY})


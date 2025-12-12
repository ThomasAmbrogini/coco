add_library(stm32_common INTERFACE)
target_include_directories(stm32_common
    INTERFACE
        cmsis
)

set(ROS_STM32_MICRO "STM32F411xE" CACHE STRING "The stm target micro")

if (ROS_STM32_MICRO STREQUAL "STM32F411xE")
    add_subdirectory(f4)
else ()
    message(FATAL_ERROR "ROS_STM32_MICRO (${ROS_STM32_MICRO}) not supported")
endif()


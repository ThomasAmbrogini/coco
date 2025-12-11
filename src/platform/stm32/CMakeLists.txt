add_library(stm32_common INTERFACE)
target_include_directories(stm32_common
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        core
)

if (ROS_STM32_MICRO STREQUAL "STM32F411xE")
    add_subdirectory(f4)
endif()


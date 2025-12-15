add_library(src_common INTERFACE)
target_include_directories(src_common
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ROS_PLATFORM_VALUES "linux" "stm32")
list(JOIN ROS_PLATFORM_VALUES ", " ROS_PLATFORM_VALUES_LIST)

set(ROS_PLATFORM "linux" CACHE STRING "Target platform for build (Values: ${ROS_PLATFORM_VALUES_LIST})")
set_property(CACHE ROS_PLATFORM PROPERTY STRINGS
    ${ROS_PLATFORM_VALUES}
)

if (NOT ROS_PLATFORM IN_LIST ROS_PLATFORM_VALUES)
    list(JOIN ROS_PLATFORM_VALUES "\n  - " ROS_PLATFORM_FORMATTED_LIST)
    message(FATAL_ERROR
        "Invalid ROS_PLATFORM: ${ROS_PLATFORM}\n"
        "Valid options are: \n"
        "  - ${ROS_PLATFORM_FORMATTED_LIST}"
    )
endif()

message(STATUS "Building for platform: " ${ROS_PLATFORM})
add_subdirectory("platform/${ROS_PLATFORM}")


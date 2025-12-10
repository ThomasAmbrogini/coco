message("Building for platform: " ${ROS_PLATFORM})

if (ROS_PLATFORM STREQUAL "Linux")
    add_subdirectory("platform/linux")
elseif (ROS_PLATFORM STREQUAL "stm32")
    add_subdirectory("platform/stm32")
else ()
    message(FATAL_ERROR "The platform is not supported")
endif()


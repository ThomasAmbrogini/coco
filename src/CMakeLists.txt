if (ROS_PLATFORM STREQUAL "Linux")
    add_subdirectory("platform/linux")
else(ROS_PLATFORM STREQUAL "stm32")
    add_subdirectory(temp_humidity_sensor)
    add_subdirectory("platform/stm32")
endif()


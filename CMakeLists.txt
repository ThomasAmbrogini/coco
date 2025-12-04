cmake_minimum_required(VERSION 3.25)

project(temperature
    VERSION 0.0.2
    DESCRIPTION "Temperature/humidity DHT22 and STM32F411"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ROS_PLATFORM)
    message("ROS_PLATFORM value: " ${ROS_PLATFORM})
else()
    message(FATAL_ERROR "ROS_PLATFORM is not set")
endif()

add_subdirectory(src)


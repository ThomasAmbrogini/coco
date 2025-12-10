cmake_minimum_required(VERSION 3.28)

project(temperature
    VERSION 0.0.2
    DESCRIPTION "Temperature/humidity DHT22 and STM32F411"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROS_PLATFORM "Linux" CACHE STRING "Target platform for build")

add_subdirectory(src)


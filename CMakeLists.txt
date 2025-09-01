cmake_minimum_required(VERSION 3.25)

project(temperature
    VERSION 0.0.1
    DESCRIPTION "Temperature/humidity DHT22 and STM32F411"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)


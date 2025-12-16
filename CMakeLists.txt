cmake_minimum_required(VERSION 4.0)

project(temperature
    VERSION 0.0.2
    DESCRIPTION "Temperature/humidity DHT22 and STM32F411"
    LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(project_warnings)
include(project_options)

add_library(ProjectWarnings INTERFACE)
set_interface_target_warnings(ProjectWarnings)

add_library(ProjectOptions INTERFACE)
set_interface_target_options(ProjectOptions)

add_subdirectory(src)


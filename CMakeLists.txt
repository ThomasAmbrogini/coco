cmake_minimum_required(VERSION 4.0)

project(temperature
    VERSION 0.0.2
    DESCRIPTION "Temperature/humidity DHT22 and STM32F411"
    LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(project_warnings)
get_project_warnings(PROJECT_WARNINGS)
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings
    INTERFACE
        ${PROJECT_WARNINGS}
)

add_subdirectory(src)


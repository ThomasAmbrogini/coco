cmake_minimum_required(VERSION 4.0)

project(coco
    VERSION 0.0.2
    DESCRIPTION "Bare metal constexpr hardware abstraction"
    LANGUAGES C CXX ASM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(project_warnings)
include(project_options)

add_library(ProjectWarnings INTERFACE)
set_interface_target_warnings(ProjectWarnings)

add_library(ProjectOptions INTERFACE)
set_interface_target_options(ProjectOptions)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include/
)

add_subdirectory(src)


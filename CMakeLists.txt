cmake_minimum_required(VERSION 4.0)

project(coco
    VERSION 0.0.2
    DESCRIPTION "Bare metal constexpr hardware abstraction"
    LANGUAGES C CXX ASM
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(coco INTERFACE)
target_sources(coco
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include/ src/
)

include(project_options)
include(project_warnings)

add_library(options INTERFACE)
set_interface_target_options(options)

add_library(warnings INTERFACE)
set_interface_target_warnings(warnings)

set(BUILD_EXAMPLES "OFF" CACHE BOOL "Set to to true if you want to build the examples.")

if(${BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()


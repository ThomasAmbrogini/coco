cmake_minimum_required(VERSION 4.0)

project(coco
    VERSION 0.0.2
    DESCRIPTION "Bare metal constexpr hardware abstraction"
    LANGUAGES C CXX ASM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(coco INTERFACE)
target_sources(coco
    INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include/
)


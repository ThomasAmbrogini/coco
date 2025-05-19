cmake_minimum_required(VERSION 3.25)

project(cocoboard
    VERSION 0.0.1
    DESCRIPTION "Keyboard implemented with STM32F411"
    LANGUAGES C CXX)

message("C flags: ${CMAKE_C_FLAGS}")
message("C++ flags: ${CMAKE_CXX_FLAGS}")

add_executable(cocoboard vector_table.cpp main.cpp)
target_compile_options(cocoboard PRIVATE "-save-temps" "-fno-exceptions")
target_compile_options(cocoboard PRIVATE "-ffunction-sections" "-fdata-sections")

target_link_options(cocoboard PRIVATE "-Wl,--gc-sections" "-Wl,--print-gc-sections")
target_link_options(cocoboard PUBLIC -T${CMAKE_SOURCE_DIR}/stm32f411.ld)

add_subdirectory(src)

#TODO: this will have to be added sooner or later.
#target_link_libraries(cocoboard gcc)


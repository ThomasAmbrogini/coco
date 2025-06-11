set(SOURCES temperature_humidity_sensor.cpp)

add_library(temp_humidity STATIC ${SOURCES})
target_include_directories(temp_humidity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(temp_humidity PRIVATE "-save-temps" "-fno-exceptions"
    "-g" "-O0")
target_compile_options(temp_humidity PRIVATE "-ffunction-sections"
    "-fdata-sections")
target_link_libraries(temp_humidity PRIVATE stm_interface)
target_link_libraries(temp_humidity PRIVATE gcc)
target_link_libraries(temp_humidity PRIVATE c)

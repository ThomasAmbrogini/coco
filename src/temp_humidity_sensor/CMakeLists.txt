set(SOURCES temperature_humidity_sensor.cpp)

add_library(temp_humidity STATIC ${SOURCES})
target_include_directories(temp_humidity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

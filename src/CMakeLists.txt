target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(COCO_PLATFORM_VALUES "linux" "stm32")
list(JOIN COCO_PLATFORM_VALUES ", " COCO_PLATFORM_VALUES_LIST)

set(COCO_PLATFORM "linux" CACHE STRING "Target platform for build (Values: ${COCO_PLATFORM_VALUES_LIST})")
set_property(CACHE COCO_PLATFORM PROPERTY STRINGS
    ${COCO_PLATFORM_VALUES}
)

if (NOT COCO_PLATFORM IN_LIST COCO_PLATFORM_VALUES)
    list(JOIN COCO_PLATFORM_VALUES "\n  - " COCO_PLATFORM_FORMATTED_LIST)
    message(FATAL_ERROR
        "Invalid COCO_PLATFORM: ${COCO_PLATFORM}\n"
        "Valid options are: \n"
        "  - ${COCO_PLATFORM_FORMATTED_LIST}"
    )
endif()

message(STATUS "Building for platform: " ${COCO_PLATFORM})
add_subdirectory("platform/${COCO_PLATFORM}")


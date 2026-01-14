target_sources(${PROJECT_NAME}
    PRIVATE
        libc/libc_runtime.cpp

    PRIVATE
        FILE_SET HEADERS
        BASE_DIRS
            cmsis
)

set(COCO_STM32_MICRO_VALUES "stm32f411")
list(JOIN COCO_STM32_MICRO_VALUES ", " COCO_STM32_MICRO_LIST)
set(COCO_STM32_MICRO "stm32f411" CACHE STRING "The stm target micro (Values: ${COCO_STM32_MICRO_VALUES})")

if (NOT COCO_STM32_MICRO IN_LIST COCO_STM32_MICRO_VALUES)
    list(JOIN COCO_STM32_MICRO_VALUES "\n  - " COCO_STM32_MICRO_FORMATTED_LIST)
    message(FATAL_ERROR
        "Inavlid COCO_STM32_MICRO: ${COCO_STM32_MICRO}\n"
        "Valid options are: \n"
        "  - ${COCO_STM32_MICRO_FORMATTED_LIST}"
    )
endif()

message(STATUS "Selected STM micro: ${COCO_STM32_MICRO}")

include(stm32_utils)

stm32_mcu_macro(${COCO_STM32_MICRO} MCU_MACRO)
if(NOT MCU_MACRO)
    message(FATAL_ERROR
        "The MCU_MACRO for ${COCO_STM32_MICRO} does not exist"
    )
endif()

stm32_micro_extract_family(${COCO_STM32_MICRO} STM32_MICRO_FAMILY)

add_subdirectory(${STM32_MICRO_FAMILY})


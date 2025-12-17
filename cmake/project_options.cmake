#
# Functions related to project options
#

function(set_interface_target_options target_name)
    target_compile_options(${target_name}
        INTERFACE
            -ffunction-sections
            -fdata-sections
            -save-temps
            $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    )

    target_link_options(${target_name}
        INTERFACE
            "LINKER:--gc-sections"
    )

    target_compile_definitions(${target_name}
        INTERFACE
            $<$<NOT:$<CONFIG:Release>>:DEBUG>
    )
endfunction()


#
# Functions related to project options
#

function(set_interface_target_options target_name)
    target_compile_options(${target_name}
        INTERFACE
            "-save-temps"
            "-g"
            "-O0"

        INTERFACE
            "-ffunction-sections"
            "-fdata-sections"
            $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    )

    target_link_options(${target_name}
        INTERFACE
            "LINKER:--gc-sections"
    )
endfunction()


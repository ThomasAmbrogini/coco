#
# Functions related to project warnings
#

function(set_interface_target_warnings target_name)
    set(GNU_WARNINGS
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
    )

    target_compile_options(${target_name}
        INTERFACE
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>:${GNU_WARNINGS}>
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<C_COMPILER_ID:GNU>>:${GNU_WARNINGS}>
    )
endfunction()


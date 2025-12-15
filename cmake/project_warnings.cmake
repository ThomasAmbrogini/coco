#
# The warnings enabled generally for the project
#

function(get_project_warnings project_warnings_var)
    set(${project_warnings_var}
        -Wall
        -Wextra
    )

    return(PROPAGATE ${project_warnings_var})
endfunction()

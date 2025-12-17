target_sources(${PROJECT_NAME}
    PRIVATE
        linux_main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ProjectWarnings
        ProjectOptions
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}/
)


target_sources(${PROJECT_NAME}
    PRIVATE
        linux_main.cpp
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        LINUX_PLATFORM
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


add_executable(TemperatureTaker)

target_sources(TemperatureTaker
    PRIVATE
        linux_main.cpp
)

target_link_libraries(TemperatureTaker
    PRIVATE
        ProjectWarnings
)

install(TARGETS TemperatureTaker
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}/
)


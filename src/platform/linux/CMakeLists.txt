add_executable(TemperatureTaker)

target_sources(TemperatureTaker
    PRIVATE
        linux_main.cpp
)

install(TARGETS TemperatureTaker
        RUNTIME
            DESTINATION ${CMAKE_INSTALL_BINDIR}/)

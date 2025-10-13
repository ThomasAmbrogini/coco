#pragma once

#include "stm32_uart.h"

namespace log {

enum class Level {
    Debug,
    Info,
    Error,
    NoLog,
};

inline constexpr Level log_level = Level::Info;

namespace {
    using uart_write_msg = void(*)(const char*, int);
    constexpr uart_write_msg uart_write = uart::write<uart::Instance::_2, uart::FrameBits::_8>;
}

template<Level _level>
inline void log(const char* msg, int size) {
    if constexpr (log_level <= _level) {
        uart_write(msg, size);
    }
}

inline constexpr auto debug = log<Level::Debug>;
inline constexpr auto info  = log<Level::Info>;
inline constexpr auto error = log<Level::Error>;

} /* namespace log */


#pragma once

#include "stm32_uart.h"

namespace log {

struct Descriptor {
    int start_idx;
    int end_idx;
};

enum class Level {
    Debug,
    Info,
    Error,
    NoLog,
};

inline constexpr Level log_level = Level::Info;

static char s_log_queue[1000] {};
static Descriptor s_descriptors[100] {};

static int s_log_head {};
static int s_log_tail {};

static int s_descriptor_head {};
static int s_descriptor_tail {};

namespace {
    using uart_write_msg = void(*)(const char*, int);
    constexpr uart_write_msg uart_write = uart::write<uart::Instance::_2, uart::FrameBits::_8>;
}

inline void memcpy(void* dst, const void* src, int size) {
    int i = 0;
    while (size > 0) {
        ((uint8_t*) dst)[i] = ((uint8_t*) src)[i];
        ++i;
        --size;
    }
}

template<Level _level>
inline void store_log(const char* msg, int size) {
    if constexpr (log_level <= _level) {
        memcpy(&s_log_queue[s_log_tail], msg, size);
        int log_start_idx = s_log_tail;

        if ((s_log_tail + size) > (1000 - 1)) {
            s_log_tail = size - (999 - s_log_tail);
        } else {
            s_log_tail += size;
        }

        Descriptor descriptor {log_start_idx, s_log_tail};

        s_descriptors[s_descriptor_tail] = descriptor;
        s_descriptor_tail = (s_descriptor_tail + 1) % 100;
    }
}

inline void console_flush() {
    while(s_descriptor_head != s_descriptor_tail) {
        int log_start_idx = s_descriptors[s_descriptor_head].start_idx;
        int log_end_idx = s_descriptors[s_descriptor_head].end_idx;
        //TODO: this is only true if log_end_idx > log_start_idx
        int size = log_end_idx - log_start_idx;
        const char* msg = &s_log_queue[log_start_idx];
        uart_write(msg, size);
        s_descriptor_head = (s_descriptor_head + 1) % 100;
    }
}

inline constexpr auto debug = store_log<Level::Debug>;
inline constexpr auto info  = store_log<Level::Info>;
inline constexpr auto error = store_log<Level::Error>;

} /* namespace log */


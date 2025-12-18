#pragma once

#include "drivers/uart/stm32_uart.h"
#include "print/print.h"
#include "print/sink.h"

using uart_write_msg = void(*)(const char*, int);
constexpr uart_write_msg uart_write = uart::write<uart::Instance::_2, uart::FrameBits::_8>;

namespace print {

void register_uart_sink() {
    Sink uart_sink {
        .write = uart_write,
        .valid = true,
    };

    print::register_sink(uart_sink);
}

} /* namespace print */


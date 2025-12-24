#pragma once

#include "drivers/uart/stm32_uart.h"
#include "print/print.h"
#include "print/sink.h"
#include "ros/string_view.h"

using uart_write_msg = void(*)(ros::StringView data);
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


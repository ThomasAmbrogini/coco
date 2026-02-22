#pragma once

#include "coco/drivers/clk/st/stm32f4_clk_types.hh"
#include "coco/drivers/uart/st/stm32f4_uart_types.hh"

namespace coco {

struct device_info {
    clk::clock_config ClockConfig;
    uart::uart_config UartConfig[3];
};

} /* namespace coco */


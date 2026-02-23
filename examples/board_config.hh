#pragma once

#include "coco/device/st/stm32f4_device.hh"

#include "vendor/st/ll/stm32f4xx_ll_rcc.h"

namespace config {

inline constexpr coco::device_info GlobalDeviceInfo {
    .ClockConfig {
        .ExternalClockFreqHz {8000000},
        .InternalClockFreqHz {HSI_VALUE},
        .ClockTree {
            .SysclkFreqHz {100000000},
            .AHBFreqHz {100000000},
            .APB1FreqHz {50000000},
            .APB2FreqHz {100000000},
        },
        .ClockSource { clk::clock_source::PLL_HSE },
    },
    .UartConfig {
        {
        },
        {
        },
        {
            .DataBits {uart::data_bits::_8},
            .StopBits {uart::stop_bits::_1},
        },
    },
};

} /* namespace config */


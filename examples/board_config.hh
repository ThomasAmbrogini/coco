#pragma once

#include "coco/device/st/stm32f4_device.hh"

inline constexpr coco::device_info GlobalDeviceInfo {
    .ClockConfig {
        .ExternalClockFreqHz {8000000},
        .ClockTree {
            .SysclkFreqHz {100000000},
            .AHBFreqHz {100000000},
            .APB1FreqHz {50000000},
            .APB2FreqHz {100000000},
        },
        .ClockSource { clk::clock_source::PLL_HSE },
    },
};

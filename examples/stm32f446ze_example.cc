#include "coco/device/st/stm32f4_device.hh"
#include "coco/drivers/clk/st/stm32f4_clk.hh"

int main() {
    static constexpr coco::device_info DeviceInfo {
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

    clk::clock_configuration<DeviceInfo>();
}


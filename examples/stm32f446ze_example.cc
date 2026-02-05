#include "coco/device/st/stm32f4_device.hh"
#include "coco/drivers/clk/st/stm32f4_clk.hh"
#include "coco/drivers/uart/st/stm32f4_uart.hh"

namespace uart {

template<int _PeriphFreqHz>
void configuration() {
    static constexpr instance UsartInstance = uart::instance::_2;
    enable_clock<UsartInstance>();
    gpio_pin_configuration<UsartInstance>();
    enable<UsartInstance>();
    set_data_bits<UsartInstance, data_bits::_8>();
    set_stop_bits<UsartInstance, stop_bits::_1>();

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    NVIC_EnableIRQ(USART2_IRQn);
    //TODO: what priority for the interrupt?
    NVIC_SetPriority(USART2_IRQn, 5);

    //TODO: pass this through the configuration
    static constexpr int DesiredBaudRate {9600};
    //TODO: change the value for the divider and the baud rate and place them in a system file.
    //TODO: the value of the clock used depends on the peripheral.

    set_baudrate<UsartInstance, DesiredBaudRate, _PeriphFreqHz, 16>();
    enable_tx<UsartInstance>();
}

} /* namespace uart */

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

    //TODO: this should be chosen based on the instance.
    uart::configuration<DeviceInfo.ClockConfig.ClockTree.APB1FreqHz>();
}


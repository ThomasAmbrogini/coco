#include "coco/drivers/clk/st/stm32f4_clk.hh"
#include "coco/drivers/uart/st/stm32f4_uart.hh"
#include "coco/print/printr.hh"

#include "board_config.hh"

namespace uart {

template<instance _UsartInstance>
void gpio_pin_configuration() {
    if constexpr (_UsartInstance == instance::_3) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
        LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_8, LL_GPIO_AF_7);
    } else {
        static_assert(false, "Implement GPIO configuration for usart instance");
    }
}

void uart_configuration() {
    static constexpr uart::instance UsartInstance {uart::instance::_3};
    static constexpr uart::uart_config UartConfig {config::GlobalDeviceInfo.UartConfig[static_cast<int>(UsartInstance)]};

    enable_clock<UsartInstance>();
    //TODO: I want this out of the uart namespace and into the GPIO (or pin) one.
    gpio_pin_configuration<UsartInstance>();
    enable_peripheral<UsartInstance>();
    //TODO: make this info into a global config option
    set_data_bits<UsartInstance, UartConfig.DataBits>();
    //TODO: make this info into a global config option
    set_stop_bits<UsartInstance, UartConfig.StopBits>();

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    //NVIC_EnableIRQ(USART2_IRQn);
    //TODO: what priority for the interrupt?
    //NVIC_SetPriority(USART2_IRQn, 5);

    //TODO: change the value for the divider and the baud rate and place them in a system file.
    //TODO: the value of the clock used depends on the peripheral.

    set_baudrate<UsartInstance, UartConfig.DesiredBaudRate, clk::GlobalClockTree.APB1FreqHz, 16>();
    enable_tx<UsartInstance>();
}

}

int main() {
    clk::clock_configuration();

    //TODO: this should be chosen based on the instance.
    uart::uart_configuration();

    printr_info("hello");

    while(true) {
    }
}


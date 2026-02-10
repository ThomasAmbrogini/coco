#include "coco/drivers/clk/st/stm32f4_clk.hh"
#include "coco/drivers/uart/st/stm32f4_uart.hh"
#include "board_config.hh"

template<int _PeriphFreqHz>
void uart_configuration() {
    static constexpr uart::instance UsartInstance = uart::instance::_2;

    uart::enable_clock<UsartInstance>();
    //TODO: I want this out of the uart namespace and into the GPIO (or pin) one.
    uart::gpio_pin_configuration<UsartInstance>();
    uart::enable_peripheral<UsartInstance>();
    uart::set_data_bits<UsartInstance, uart::data_bits::_8>();
    uart::set_stop_bits<UsartInstance, uart::stop_bits::_1>();

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    NVIC_EnableIRQ(USART2_IRQn);
    //TODO: what priority for the interrupt?
    NVIC_SetPriority(USART2_IRQn, 5);

    //TODO: pass this through the configuration
    static constexpr int DesiredBaudRate {9600};
    //TODO: change the value for the divider and the baud rate and place them in a system file.
    //TODO: the value of the clock used depends on the peripheral.

    uart::set_baudrate<UsartInstance, DesiredBaudRate, _PeriphFreqHz, 16>();
    uart::enable_tx<UsartInstance>();
}

int main() {
    constexpr clk::clock_tree clk_info {clk::detail::clock<>::ActualClockTree_};

    clk::clock_configuration<GlobalDeviceInfo>();

    //TODO: this should be chosen based on the instance.
    uart_configuration<GlobalDeviceInfo.ClockConfig.ClockTree.APB1FreqHz>();
}


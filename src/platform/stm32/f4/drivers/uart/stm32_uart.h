#pragma once

#include "ros/string_view.h"
#include "stm32_uart_types.h"
#include "stm32_uart_utils.h"

#include "drivers/clk/stm32_clock.h"
#include "drivers/ll/stm32f4xx_ll_usart.h"

namespace uart {

namespace impl {

inline ros::StringView g_interrupt_data_msg {};

} /* namespace impl */

//TODO: should i just pass the known values as template params?
//maybe it is better than using them directly from the global value.
template<Instance _usart_instance, Mode _mode>
void configuration() {
    USART_TypeDef* usart_reg = convert_inst_to_real_periph<_usart_instance>();

    enable_clock<_usart_instance>();

    gpio_pin_configuration<_usart_instance>();

    LL_USART_Enable(usart_reg);

    LL_USART_SetDataWidth(usart_reg, LL_USART_DATAWIDTH_8B);

    LL_USART_SetStopBitsLength(usart_reg, LL_USART_STOPBITS_1);

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    if constexpr (_mode == Mode::Interrupt) {
        //TODO: interrupt priority.
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
    }

    static constexpr int desired_baud_rate {9600};
    //TODO: change the value for the divider and the baud rate and place them in a system file.
    //TODO: the value of the clock used depends on the peripheral.
    static constexpr uint16_t brr_reg_val {compute_brr_val<desired_baud_rate, clk::apb1_freq_hz, 16>()};
    usart_reg->BRR = brr_reg_val;

    LL_USART_EnableDirectionTx(usart_reg);
}

//TODO: i do not know if passing the framebits is the right move, because it
//relies on the user to specify the same info of the configuration.
//maybe it is better to have the frame_bits to be something global which is
//used for the configuration and as parameter inside here.
template<Instance _usart_instance, FrameBits _frame_bits>
void write(char frame_data) {
    USART_TypeDef* usart_reg = convert_inst_to_real_periph<_usart_instance>();

    //TODO: timeout which returns a fail if the write can not be done.
    while(!LL_USART_IsActiveFlag_TXE(usart_reg)) {
    }

    if constexpr (_frame_bits == FrameBits::_8) {
        LL_USART_TransmitData8(usart_reg, frame_data);
    } else if constexpr (_frame_bits == FrameBits::_9) {
        static_assert(false, "The write is not yet implemented for frame size of 9");
    }
}

//TODO: i do not know if passing the framebits is the right move, because it
//relies on the user to specify the same info of the configuration.
//maybe it is better to have the frame_bits to be something global which is
//used for the configuration and as parameter inside here.
template<Mode _mode, Instance _usart_instance, FrameBits _frame_bits>
void write(ros::StringView data) {
    if constexpr (_mode == Mode::Blocking) {
        //TODO: timeout which returns a fail if the write can not be done.
        for (int i = 0; i < data.size(); ++i) {
            write<_usart_instance, _frame_bits>(data.data()[i]);
        }
    }
    else if constexpr (_mode == Mode::Interrupt) {
        impl::g_interrupt_data_msg = data;
        USART_TypeDef* usart_reg = convert_inst_to_real_periph<_usart_instance>();
        LL_USART_EnableIT_TXE(usart_reg);
    }
}

template<Instance _usart_instance>
void end_transmission() {
    USART_TypeDef* usart_reg = convert_inst_to_real_periph<_usart_instance>();

    while (LL_USART_IsActiveFlag_TC(usart_reg)) {
    }
}

} /* namespace uart */


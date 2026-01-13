#pragma once

#include "ros/string_view.h"
#include "stm32_uart_types.h"
#include "stm32_uart_utils.h"

#include "drivers/clk/stm32_clock.h"
#include "drivers/ll/stm32f4xx_ll_usart.h"

namespace uart {

namespace impl {

inline ros::string_view GInterruptDataMsg {};

} /* namespace impl */

//TODO: should i just pass the known values as template params?
//maybe it is better than using them directly from the global value.
template<instance _UsartInstance, mode _Mode>
void configuration() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    enable_clock<_UsartInstance>();

    gpio_pin_configuration<_UsartInstance>();

    LL_USART_Enable(UsartReg);

    LL_USART_SetDataWidth(UsartReg, LL_USART_DATAWIDTH_8B);

    LL_USART_SetStopBitsLength(UsartReg, LL_USART_STOPBITS_1);

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    if constexpr (_Mode == mode::Interrupt) {
        //TODO: interrupt priority.
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 5);
    }

    static constexpr int DesiredBaudRate {9600};
    //TODO: change the value for the divider and the baud rate and place them in a system file.
    //TODO: the value of the clock used depends on the peripheral.
    static constexpr uint16_t BrrRegVal {compute_brr_val<DesiredBaudRate, clk::APB1FreqHz, 16>()};
    UsartReg->BRR = BrrRegVal;

    LL_USART_EnableDirectionTx(UsartReg);
}

//TODO: i do not know if passing the framebits is the right move, because it
//relies on the user to specify the same info of the configuration.
//maybe it is better to have the frame_bits to be something global which is
//used for the configuration and as parameter inside here.
template<instance _UsartInstance, frame_bits _FrameBits>
void write(char FrameData) {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    //TODO: timeout which returns a fail if the write can not be done.
    while(!LL_USART_IsActiveFlag_TXE(UsartReg)) {
    }

    if constexpr (_FrameBits == frame_bits::_8) {
        LL_USART_TransmitData8(UsartReg, FrameData);
    } else if constexpr (_FrameBits == frame_bits::_9) {
        static_assert(false, "The write is not yet implemented for frame size of 9");
    }
}

//TODO: i do not know if passing the framebits is the right move, because it
//relies on the user to specify the same info of the configuration.
//maybe it is better to have the frame_bits to be something global which is
//used for the configuration and as parameter inside here.
template<mode _Mode, instance _UsartInstance, frame_bits _FrameBits>
void write(ros::string_view Data) {
    if constexpr (_Mode == mode::Blocking) {
        //TODO: timeout which returns a fail if the write can not be done.
        for (int I = 0; I < Data.size(); ++I) {
            write<_UsartInstance, _FrameBits>(Data.data()[I]);
        }
    }
    else if constexpr (_Mode == mode::Interrupt) {
        impl::GInterruptDataMsg = Data;
        USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();
        LL_USART_EnableIT_TXE(UsartReg);
    }
}

template<instance _UsartInstance>
void end_transmission() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    while (LL_USART_IsActiveFlag_TC(UsartReg)) {
    }
}

} /* namespace uart */


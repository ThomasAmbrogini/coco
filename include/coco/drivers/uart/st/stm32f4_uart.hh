#pragma once

#include "coco/string_view.hh"
#include "coco/drivers/clk/st/stm32f4_clk.hh"
#include "stm32f4_uart_types.hh"
#include "stm32f4_uart_utils.hh"

#include "vendor/st/ll/stm32f4xx_ll_usart.h"

namespace uart {

namespace impl {

inline coco::string_view GInterruptDataMsg {};

} /* namespace impl */

//TODO: do I want this function to reside in the clock module?
template<instance _UsartInstance>
inline void enable_clock() {
    if constexpr (_UsartInstance == instance::_2) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    } else {
        static_assert(false, "Implement clock enable for the periph");
    }
}

template<instance _UsartInstance>
void enable() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();
    LL_USART_Enable(UsartReg);
}

//TODO: move this thing to the gpio file.
template<instance _UsartInstance>
void gpio_pin_configuration() {
    if constexpr (_UsartInstance == instance::_2) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);
    } else {
        static_assert(false, "Implement GPIO configuration for usart instance");
    }
}

//TODO: i do not know if passing the framebits is the right move, because it
//relies on the user to specify the same info of the configuration.
//maybe it is better to have the data_bits to be something global which is
//used for the configuration and as parameter inside here.
template<instance _UsartInstance, data_bits _DataBits>
void write(char FrameData) {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    //TODO: timeout which returns a fail if the write can not be done.
    while(!LL_USART_IsActiveFlag_TXE(UsartReg)) {
    }

    if constexpr (_DataBits == data_bits::_8) {
        LL_USART_TransmitData8(UsartReg, FrameData);
    } else if constexpr (_DataBits == data_bits::_9) {
        static_assert(false, "The write is not yet implemented for frame size of 9");
    }
}

template<instance _UsartInstance, data_bits _DataBits>
void set_data_bits() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    if constexpr (_DataBits == data_bits::_8) {
        LL_USART_SetDataWidth(UsartReg, LL_USART_DATAWIDTH_8B);
    } else if constexpr (_DataBits == data_bits::_9) {
        LL_USART_SetDataWidth(UsartReg, LL_USART_DATAWIDTH_9B);
    }
}

template<instance _UsartInstance, stop_bits _StopBits>
void set_stop_bits() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    if constexpr (_StopBits == stop_bits::_1) {
        LL_USART_SetStopBitsLength(UsartReg, LL_USART_STOPBITS_1);
    } else if constexpr (_StopBits == stop_bits::_2) {
        LL_USART_SetStopBitsLength(UsartReg, LL_USART_STOPBITS_2);
    } else {
        static_assert(false, "Yet to implement");
    }
}

template<instance _UsartInstance, int _DesiredBaudRate, int _PeriphFreqHz, int _OverSampling>
void set_baudrate() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    static constexpr uint16_t BrrRegVal {compute_brr_val<_DesiredBaudRate, _PeriphFreqHz, _OverSampling>()};
    UsartReg->BRR = BrrRegVal;
}

template<instance _UsartInstance>
void enable_tx() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();
    LL_USART_EnableDirectionTx(UsartReg);
}

//TODO: i do not know if passing the framebits is the right move, because it
//relies on the user to specify the same info of the configuration.
//maybe it is better to have the data_bits to be something global which is
//used for the configuration and as parameter inside here.
template<instance _UsartInstance, data_bits _DataBits>
void write_blocking(coco::string_view Data) {
    //TODO: timeout which returns a fail if the write can not be done.
    for (int I = 0; I < Data.size(); ++I) {
        write<_UsartInstance, _DataBits>(Data.data()[I]);
    }
}

template<instance _UsartInstance, data_bits _DataBits>
void write_interrupt(coco::string_view Data) {
    //TODO: remove this thing.
    impl::GInterruptDataMsg = Data;
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();
    LL_USART_EnableIT_TXE(UsartReg);
}

template<instance _UsartInstance>
void wait_end_transmission() {
    USART_TypeDef* UsartReg = convert_inst_to_real_periph<_UsartInstance>();

    while (LL_USART_IsActiveFlag_TC(UsartReg)) {
    }
}

} /* namespace uart */


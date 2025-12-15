#pragma once

#include "drivers/clk/stm32_clock.h"
#include "util/math.h"
#include "util/limits.h"
#include "util/type_traits.h"

#include "drivers/ll/stm32f4xx_ll_bus.h"
#include "drivers/ll/stm32f4xx_ll_gpio.h"
#include "drivers/ll/stm32f4xx_ll_usart.h"

namespace uart {

enum class Instance {
    _1,
    _2,
    _6,
};

enum class FrameBits {
    _8,
    _9,
};

template<Instance _usart_instance>
constexpr USART_TypeDef* convert_inst_to_real_periph() {
    if constexpr (_usart_instance == Instance::_2) {
        return USART2;
    } else {
        static_assert(false, "Implement things for other peripherals");
    }
}

template<float _usart_div>
constexpr uint16_t compute_brr_val() {
    static_assert(static_cast<int>(_usart_div) <= unsigned_bit_limits<12>::max());

    static constexpr uint16_t integral_part {static_cast<uint16_t>(_usart_div)};
    static constexpr float floating_part {_usart_div - integral_part};
    static constexpr int floating_part_fixed_point {round(floating_part * 16)};
    static_assert(floating_part_fixed_point <= unsigned_bit_limits<4>::max());

    return (integral_part << 4) | floating_part_fixed_point;
}

template<int _baud_rate, int _freq_clk_hz, int _oversampling>
constexpr uint16_t compute_brr_val() {
    static constexpr float usart_div = (static_cast<float>(_freq_clk_hz) / (_baud_rate * _oversampling));
    static_assert(usart_div <= unsigned_bit_limits<12>::max());
    static_assert(_baud_rate > (_freq_clk_hz / (_oversampling * unsigned_bit_limits<12>::max())), "The baud rate is lower than the minimum possible");
    return compute_brr_val<usart_div>();
}

static_assert(compute_brr_val<9600, 16000000, 16>() == compute_brr_val<104.1875f>());
static_assert(compute_brr_val<115200, 16000000, 16>() == compute_brr_val<8.6875f>());
static_assert(compute_brr_val<460800, 8000000, 16>() == compute_brr_val<1.0625f>());
static_assert(compute_brr_val<115200, 100000000, 16>() == compute_brr_val<54.253472f>());

template<Instance _usart_instance>
void gpio_pin_configuration() {
    if constexpr (_usart_instance == Instance::_2) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);
    } else {
        static_assert(false, "Implement GPIO configuration for usart instance");
    }
}

//TODO: do I want this function to reside in the clock module?
template<Instance _usart_instance>
inline void enable_clock() {
    if constexpr (_usart_instance == Instance::_2) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    } else {
        static_assert(false, "Implement clock enable for the periph");
    }
}

//TODO: should i just pass the known values as template params?
//maybe it is better than using them directly from the global value.
template<Instance _usart_instance>
void configuration() {
    USART_TypeDef* usart_reg = convert_inst_to_real_periph<_usart_instance>();

    enable_clock<_usart_instance>();

    gpio_pin_configuration<_usart_instance>();

    LL_USART_Enable(usart_reg);

    LL_USART_SetDataWidth(usart_reg, LL_USART_DATAWIDTH_8B);

    LL_USART_SetStopBitsLength(usart_reg, LL_USART_STOPBITS_1);

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

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
template<Instance _usart_instance, FrameBits _frame_bits>
void write(const char * data, int size) {
    //TODO: timeout which returns a fail if the write can not be done.
    for (int i = 0; i < size; ++i) {
        write<_usart_instance, _frame_bits>(data[i]);
    }
}

template<Instance _usart_instance>
void end_transmission() {
    USART_TypeDef* usart_reg = convert_inst_to_real_periph<_usart_instance>();

    while (LL_USART_IsActiveFlag_TC(usart_reg)) {
    }
}

} /* namespace uart */


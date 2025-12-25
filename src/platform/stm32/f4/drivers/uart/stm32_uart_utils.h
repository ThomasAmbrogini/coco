#pragma once

#include "stm32_uart_types.h"
#include "util/limits.h"

#include "drivers/ll/stm32f4xx_ll_bus.h"
#include "drivers/ll/stm32f4xx_ll_gpio.h"
#include "drivers/ll/stm32f4xx_ll_usart.h"

namespace uart {

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

} /* namespace uart */


#pragma once

#include "stm32f4_uart_types.hh"
#include "coco/util/limits.hh"
#include "coco/util/math.hh"

#include "vendor/st/ll/stm32f4xx_ll_bus.h"
#include "vendor/st/ll/stm32f4xx_ll_gpio.h"
#include "vendor/st/ll/stm32f4xx_ll_usart.h"

namespace uart {

template<instance _UsartInstance>
constexpr USART_TypeDef* convert_inst_to_real_periph() {
    if constexpr (_UsartInstance == instance::_2) {
        return USART2;
    } else {
        static_assert(false, "Implement things for other peripherals");
    }
}

template<float _UsartDiv>
constexpr uint16_t compute_brr_val() {
    static_assert(static_cast<int>(_UsartDiv) <= unsigned_bit_limits<12>::max());

    static constexpr uint16_t IntegralPart {static_cast<uint16_t>(_UsartDiv)};
    static constexpr float FloatingPart {_UsartDiv - IntegralPart};
    static constexpr int FloatingPartFixedPoint {round(FloatingPart * 16)};
    static_assert(FloatingPartFixedPoint <= unsigned_bit_limits<4>::max());

    return (IntegralPart << 4) | FloatingPartFixedPoint;
}

template<int _BaudRate, int _FreqClkHz, int _Oversampling>
constexpr uint16_t compute_brr_val() {
    static constexpr float UsartDiv = (static_cast<float>(_FreqClkHz) / (_BaudRate * _Oversampling));
    static_assert(UsartDiv <= unsigned_bit_limits<12>::max());
    static_assert(_BaudRate > (_FreqClkHz / (_Oversampling * unsigned_bit_limits<12>::max())), "The baud rate is lower than the minimum possible");
    return compute_brr_val<UsartDiv>();
}

static_assert(compute_brr_val<9600, 16000000, 16>() == compute_brr_val<104.1875f>());
static_assert(compute_brr_val<115200, 16000000, 16>() == compute_brr_val<8.6875f>());
static_assert(compute_brr_val<460800, 8000000, 16>() == compute_brr_val<1.0625f>());
static_assert(compute_brr_val<115200, 100000000, 16>() == compute_brr_val<54.253472f>());

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

//TODO: do I want this function to reside in the clock module?
template<instance _UsartInstance>
inline void enable_clock() {
    if constexpr (_UsartInstance == instance::_2) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    } else {
        static_assert(false, "Implement clock enable for the periph");
    }
}

} /* namespace uart */


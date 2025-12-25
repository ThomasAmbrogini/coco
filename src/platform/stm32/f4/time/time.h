#pragma once

#include "drivers/clk/stm32_clock.h"
#include "drivers/ll/stm32f4xx_ll_bus.h"
#include "drivers/ll/stm32f4xx_ll_tim.h"

#include <stdint.h>

namespace time {

inline constexpr uint32_t tim2_freq_hz { clk::apb1_freq_hz };
inline constexpr float ns_per_cycle { 1 / (tim2_freq_hz / 1000000000.0f) };
inline constexpr float us_per_cycle { ns_per_cycle / 1000 };
inline constexpr float ms_per_cycle { us_per_cycle / 1000 };

inline int time_init() {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_TIM_SetPrescaler(TIM2, 0);
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    LL_TIM_EnableCounter(TIM2);

    return 0;
}

inline uint32_t get_time_ms() {
    uint32_t timer_counter_value {LL_TIM_GetCounter(TIM2)};
    return timer_counter_value * ms_per_cycle;
}

inline uint32_t get_time_us() {
    uint32_t timer_counter_value {LL_TIM_GetCounter(TIM2)};
    return timer_counter_value * us_per_cycle;
}

inline uint32_t get_time_ns() {
    uint32_t timer_counter_value {LL_TIM_GetCounter(TIM2)};
    return timer_counter_value * ns_per_cycle;
}

} /* namespace time */


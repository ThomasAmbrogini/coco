#pragma once

#include "drivers/clk/stm32_clock.h"
#include "drivers/ll/stm32f4xx_ll_bus.h"
#include "drivers/ll/stm32f4xx_ll_tim.h"

#include <stdint.h>

namespace time {

inline constexpr uint32_t TIM2FreqHz { clk::APB1FreqHz };
inline constexpr float NsPerCycle { 1 / (TIM2FreqHz / 1000000000.0f) };
inline constexpr float UsPerCycle { NsPerCycle / 1000 };
inline constexpr float MsPerCycle { UsPerCycle / 1000 };

inline int time_init() {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_TIM_SetPrescaler(TIM2, 0);
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    LL_TIM_EnableCounter(TIM2);

    return 0;
}

inline uint32_t get_time_ms() {
    uint32_t timer_counter_value {LL_TIM_GetCounter(TIM2)};
    return timer_counter_value * MsPerCycle;
}

inline uint32_t get_time_us() {
    uint32_t timer_counter_value {LL_TIM_GetCounter(TIM2)};
    return timer_counter_value * UsPerCycle;
}

inline uint32_t get_time_ns() {
    uint32_t timer_counter_value {LL_TIM_GetCounter(TIM2)};
    return timer_counter_value * NsPerCycle;
}

} /* namespace time */


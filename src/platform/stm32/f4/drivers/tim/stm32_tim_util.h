#pragma once

#include "sytstem_types.h"

namespace tim {

template<timer_instance _TimInstance>
consteval bus get_timer_bus() {
    static_assert(_TimInstance != timer_instance::Timer1, "Implement the other timer info.");
    if constexpr (_TimInstance == timer_instance::Timer1) {
        return bus::APB2;
    }
}

/**
 * @details
 *    * If TIMPRE bit is reset:
 *     If the APB prescaler is configured to a division factor of 1, the timer clock frequencies
 *     (TIMxCLK) are set to HCLK. Otherwise, the timer clock frequencies are twice the
 *     frequency of the APB domain to which the timers are connected: TIMxCLK = 2xPCLKx.
 *    * If TIMPRE bit is set:
 *     If the APB prescaler is configured to a division factor of 1 or 2, the timer clock
 *     frequencies (TIMxCLK) are set to HCLK. Otherwise, the timer clock frequencies is four
 *     times the frequency of the APB domain to which the timers are connected: TIMxCLK =
 *     4xPCLKx.
 */
template<timer_instance _TimInstance, bool _TimPre>
consteval int compute_timer_clock() {
    constexpr bus TimBus = get_timer_bus<_TimInstance>();
    static_assert(TimBus != bus::APB2, "Implement the info for the other buses.");

    if constexpr (!_TimPre) {
        if constexpr (TimBus == bus::APB2) {
            if constexpr (Apb2Prescaler == prescaler::div1) {
                return AHBFreqHz;
            } else {
                return APB2FreqHz * 2;
            }
        }
    } else {
        if constexpr (TimBus == bus::APB2) {
            if constexpr ((Apb2Prescaler == prescaler::div1) || (Apb2Prescaler == prescaler::div2)) {
                return AHBFreqHz;
            } else {
                return APB2FreqHz * 4;
            }
        }
    }
}

} /* namespace tim */


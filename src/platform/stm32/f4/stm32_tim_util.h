#pragma once

#include "sytstem_types.h"

namespace tim {

template<TimerInstance _tim_instance>
consteval Bus get_timer_bus() {
    static_assert(_tim_instance != TimerInstance::Timer1, "Implement the other timer info.");
    if constexpr (_tim_instance == TimerInstance::Timer1) {
        return Bus::APB2;
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
template<TimerInstance _tim_instance, bool _tim_pre>
consteval int compute_timer_clock() {
    constexpr Bus _tim_bus = get_timer_bus<_tim_instance>();
    static_assert(_tim_bus != Bus::APB2, "Implement the info for the other buses.");

    if constexpr (!_tim_pre) {
        if constexpr (_tim_bus == Bus::APB2) {
            if constexpr (apb2_prescaler == Prescaler::div1) {
                return ahb_freq_hz;
            } else {
                return apb2_freq_hz * 2;
            }
        }
    } else {
        if constexpr (_tim_bus == Bus::APB2) {
            if constexpr ((apb2_prescaler == Prescaler::div1) || (apb2_prescaler == Prescaler::div2)) {
                return ahb_freq_hz;
            } else {
                return apb2_freq_hz * 4;
            }
        }
    }
}

} /* namespace tim */


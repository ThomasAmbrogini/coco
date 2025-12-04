#pragma once

#include "system_types.h"

namespace clk {

inline constexpr int desired_sysclk_freq_hz = 100000000;
static_assert(desired_sysclk_freq_hz <= 100000000, "The maximum frequency for the core is 100MHz");

inline constexpr Prescaler ahb_prescaler = Prescaler::div1;
static_assert(ahb_prescaler <= Prescaler::div512, "The maximum prescaler for ahb is div512");
inline constexpr Prescaler apb1_prescaler = Prescaler::div2;
static_assert(apb1_prescaler <= Prescaler::div16, "The maximum prescaler for apb1 is div16");
inline constexpr Prescaler apb2_prescaler = Prescaler::div1;
static_assert(apb2_prescaler <= Prescaler::div16, "The maximum prescaler for apb2 is div16");

inline constexpr bool clock_security_system_enabled {true};

} /* namespace clk */


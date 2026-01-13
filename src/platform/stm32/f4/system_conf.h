#pragma once

#include "system_types.h"

namespace clk {

inline constexpr int DesiredSysclkFreqHz = 100000000;
static_assert(DesiredSysclkFreqHz <= 100000000, "The maximum frequency for the core is 100MHz");

inline constexpr prescaler AhbPrescaler = prescaler::div1;
static_assert(AhbPrescaler <= prescaler::div512, "The maximum prescaler for ahb is div512");
inline constexpr prescaler Apb1Prescaler = prescaler::div2;
static_assert(Apb1Prescaler <= prescaler::div16, "The maximum prescaler for apb1 is div16");
inline constexpr prescaler Apb2Prescaler = prescaler::div1;
static_assert(Apb2Prescaler <= prescaler::div16, "The maximum prescaler for apb2 is div16");

inline constexpr bool ClockSecuritySystemEnabled {true};

} /* namespace clk */


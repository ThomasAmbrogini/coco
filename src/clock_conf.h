#pragma once

namespace clk {

inline constexpr int maximum_AHB_freq_MHz = 100;
inline constexpr int maximum_APB1_freq_MHz = 50;
inline constexpr int maximum_APB2_freq_MHz = 100;

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

void setSysClockFreq(int sysclock_freq);

int computeAHBFreq();
int computeAPB1Freq();
int computeAPB2Freq();

} /* namespace clk */


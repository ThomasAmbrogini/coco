#include "clock_conf.h"

#include "stm32f4xx_ll_rcc.h"

//TODO: maybe everything can be done in constexpr because the clock
//configuration will not be done at runtime.
namespace clk {

int SYSClock_freq {};

void setSysClockFreq(int sysclock_freq) {
    SYSClock_freq = sysclock_freq;
}

int computeAHBFreq() {
    //TODO: will i have the sysclock_freq as a global (or static) var or will i
    //pass it to the function itself?
    //I think I want to know the different values, so I am going to store it as
    //global var.
    return __LL_RCC_CALC_HCLK_FREQ(SYSClock_freq, LL_RCC_GetAHBPrescaler());
}

int computeAPB1Freq() {
    return __LL_RCC_CALC_PCLK1_FREQ(SYSClock_freq, LL_RCC_GetAPB1Prescaler());
}

int computeAPB2Freq() {
    return __LL_RCC_CALC_PCLK2_FREQ(SYSClock_freq, LL_RCC_GetAPB2Prescaler());
}

} /* namespace clk */


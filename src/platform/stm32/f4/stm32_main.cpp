#include "drivers/clk/stm32_clock.h"
#include "drivers/uart/stm32_uart.h"
#include "log/log.h"

int main() {
    //TODO: what are the things which have to be absolutely powered at the
    //beginning (something about the clock?
    //TODO: Configure Flash prefetch, Instruction cache, Data cache

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

    clk::clock_configuration<clk::ClockSource::PLL_HSE, clk::desired_sysclk_freq_hz>();
    uart::configuration<uart::Instance::_2>();

    log::info("HELLOOOO", sizeof("HELLOOOO"));
    log::debug("this will not be printed", sizeof("this will not be printed"));
    log::error("THIS IS AN ERROR", sizeof("THIS IS AN ERROR"));

    log::console_flush();

    while (true)
    {
    }
}


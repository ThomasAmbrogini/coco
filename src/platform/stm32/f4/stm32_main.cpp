#include "drivers/clk/stm32_clock.h"
#include "drivers/uart/stm32_uart.h"
#include "print/printr.h"
#include "coco/string_view.h"
#include "time/time.h"

int main() {
    //TODO: what are the things which have to be absolutely powered at the
    //beginning (something about the clock?
    //TODO: Configure Flash prefetch, Instruction cache, Data cache
    LL_FLASH_EnablePrefetch();
    LL_FLASH_EnableDataCache();
    LL_FLASH_EnableInstCache();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

    clk::clock_configuration<clk::clock_source::PLL_HSE, clk::DesiredSysclkFreqHz>();
    uart::configuration<uart::instance::_2, uart::mode::Interrupt>();

    time::time_init();

    printr_info("HELLOOOO");
    printr_info("this will not be printed");
    printr_info("THIS IS AN ERROR");

    while (true)
    {
    }
}


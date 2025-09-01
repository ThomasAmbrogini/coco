#include "clock_conf.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"

namespace clk {

void PLLConfiguration() {
    // PLL Configuration
    // PLL configuration can not change once it is enabled, so it has to be
    // configured before enabling it.
    //
    //
    // The software has to set these bits correctly to ensure that the VCO input frequency
    // ranges from 1 to 2 MHz. It is recommended to select a frequency of 2 MHz to limit
    // PLL jitter.

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 100, LL_RCC_PLLP_DIV_2);

    LL_RCC_PLL_Enable();
    while (!LL_RCC_PLL_IsReady()) {
    }

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    LL_SetSystemCoreClock(100000000);
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);
}

//TODO: this has to be removed.
void measureClockFrequency() {
    //TODO: i can also measure the frequency of the various clocks using two
    //different timers.

    /*
     * The desired clock source is selected using the MCO1PRE[2:0] and MCO1[1:0]
     * bits in the RCC clock configuration register (RCC_CFGR).
     */
    //MCO1 -> PA8
    //MCO2 -> PC9

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

    LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_9, LL_GPIO_AF_0);

    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);

    LL_RCC_ConfigMCO(LL_RCC_MCO2SOURCE_PLLCLK, LL_RCC_MCO2_DIV_1);
}

} /* namespace clk */


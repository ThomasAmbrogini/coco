#include "stm32_clock.h"

#include "drivers/ll/stm32f4xx_ll_bus.h"
#include "drivers/ll/stm32f4xx_ll_gpio.h"
#include "drivers/ll/stm32f4xx_ll_rcc.h"
#include "drivers/ll/stm32f4xx_ll_tim.h"

namespace clk {

/**
 * @details
 *    This is the way to perform Main Clock out on a pin of the system clock.
 *    It can be used to measure the actual frequency of the system clock
 *    through the oscilloscope.
 */
void measure_clock_freq() {
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

/**
 * @details
 *    This is the function which enables the timer5 mode to measure a clock
 *    with another one through the capture compare value of the clock.
 *    It is explained in the TRM as last chapter of the RCC.
 *    An ISR is needed for TIM5 though.
 */
void timer_measure_clock_freq() {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
    LL_RCC_LSI_Enable();
    while(!LL_RCC_LSI_IsReady()) {
    }

    NVIC_SetPriority(TIM5_IRQn, 4);
    NVIC_EnableIRQ(TIM5_IRQn);

    LL_TIM_IC_SetActiveInput(TIM5, LL_TIM_CHANNEL_CH4, LL_TIM_ACTIVEINPUT_DIRECTTI);
    LL_TIM_SetRemap(TIM5, LL_TIM_TIM5_TI4_RMP_LSI);

    LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH4);
    LL_TIM_ClearFlag_CC4(TIM5);
    LL_TIM_ClearFlag_UPDATE(TIM5);
    LL_TIM_EnableIT_CC4(TIM5);
    LL_TIM_EnableCounter(TIM5);
}

} /* namespace clk */


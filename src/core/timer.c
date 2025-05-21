#include "timer.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_tim.h"

int MX_TIMER_Init()
{
    LL_TIM_InitTypeDef tim_conf;
    /* init the structure */
    LL_TIM_StructInit(&tim_conf);

    /* Timer Clock Enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    /* this is 1 us with 16 Mhz clock */
    tim_conf.Autoreload = 16;

    ErrorStatus status = LL_TIM_Init(TIM2, &tim_conf);

    return status;
}

void sleepUs(int us) {
   LL_TIM_SetAutoReload(TIM2, us * 16);
   LL_TIM_EnableCounter(TIM2);

   while (LL_TIM_IsActiveFlag_UPDATE(TIM2) != 1) {
   }

   LL_TIM_DisableCounter(TIM2);
}


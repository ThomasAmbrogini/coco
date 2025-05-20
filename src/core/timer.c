#include "timer.h"

void MX_TIMER_Init()
{
    LL_TIM_InitTypeDef tim_conf;
    /* init the structure */
    LL_TIM_StructInit(&tim_conf);

    /* Timer Clock Enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    //TODO: this has to contain the value to count to 1 us.
    tim_conf.Autoreload = ;

    ErrorStatus status = LL_TIM_Init(TIM2, &tim_conf);

    if (status == SUCCESS)
    {
    }
}


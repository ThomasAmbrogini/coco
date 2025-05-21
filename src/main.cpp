#include "common.h"
#include "gpio.h"
#include "timer.h"

void SystemClock_Config(void);

int main() {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    int ret = MX_TIMER_Init();

    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
    sleepUs(30);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);

    while (true)
    {
    }
}

/**
  * @brief Timer configuration for us delays.
  * @retval None
  */
//void uSTimerConfig(void)
//{
//    init1usTick(16000000);
//    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
//}
//
//void init1usTick(uint32_t clock_frequency_hz)
//{
//    initTick(clock_frequency_hz, 1000000);
//}

void initTick(uint32_t clock_frequency_hz, uint32_t ticks)
{
    //TODO: change the systick to an actual timer.
    /* Configure the SysTick to have interrupt in 1ms time base */
    SysTick->LOAD  = (uint32_t)((clock_frequency_hz / ticks) - 1UL);  /* set reload register */
    SysTick->VAL   = 0UL;                                       /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_ENABLE_Msk;                   /* Enable the Systick Timer */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(16000000);
  LL_SetSystemCoreClock(16000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /*
   * User can add his own implementation to report the file name and line number,
   * ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
   */
}
#endif


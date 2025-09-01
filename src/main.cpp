#include "common.h"
#include "clock_conf.h"
#include "gpio.h"
#include "temperature_humidity_sensor.h"
#include "timer.h"

void SystemClock_Config(void);

int main() {
    //TODO: what are the things which have to be absolutely powered at the
    //beginning (something about the clock?

    //TODO: initialize flash latency.

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

    /* Configure the system clock */
    //SystemClock_Config();

    //TODO: I would like to have an assert on the frequeuncy of the clock to be
    //correct.
    //Can I also made it to a static_assert()?

    //int ret = MX_TIMER_Init();

    /* Initialize all configured peripherals */
    //MX_GPIO_Init();
    //TODO: i need to do something in case there is no sensor.
    //volatile temp::HumTempReading reading = temp::read();

    //measureClockFrequency();
    clk::clockConfiguration<clk::ClockSource::PLL, 100000000>();
    //clk::PLLConfiguration();

    while (true)
    {
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0) {
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

void HardFault_Handler(void) {
    volatile uint32_t* cfsr = &SCB->CFSR;
    volatile uint32_t* hfsr = &SCB->HFSR;
    while (1); // trap
}


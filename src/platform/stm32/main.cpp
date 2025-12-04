#include "common.h"
#include "gpio.h"
#include "logger.h"
#include "stm32_clock.h"
#include "stm32_uart.h"
#include "temperature_humidity_sensor.h"
#include "timer.h"

void SystemClock_Config(void);

int main() {
    //TODO: what are the things which have to be absolutely powered at the
    //beginning (something about the clock?

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


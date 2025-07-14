#include "common.h"
#include "clock_conf.h"
#include "gpio.h"
#include "temperature_humidity_sensor.h"
#include "timer.h"

void SystemClock_Config(void);

void clockConfiguration(void) {
    /**
     * There are three different sources which can be selected for the stm32f411e.
     *      1. HSI
     *      2. HSE
     *      3. PLL
     *
     * The system clock can be output on a pin with the MCO2 pin.
     *
     *
     */

    /* Do i need to power the RCC? */

    /* The stm32f4 discovery has the X2 on-board oscillator which can be used for external.*/

    /* hse_ready is not palced to true until the HSE is not turned on. */
    if (!LL_RCC_HSE_IsOn()) {
        LL_RCC_HSE_Enable();

        /*
         * I can switch to a clock only when it is read, but it seems that even if
         * i do it before it is actually ready, it will wait until it is ready before
         * changing the system clock to the specified source.
         */
        while(!LL_RCC_HSE_IsReady()) {
        }

        /* hse on is true in here. */
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);

    volatile uint32_t hsi_on = LL_RCC_HSI_IsOn();
    if (LL_RCC_HSI_IsOn()) {
        LL_RCC_HSI_Disable();
    }

    //TODO: i can now which clock is current used as system clock in the cr.

    //TODO: there is a safety mechanisms which says something when the clock fails.
    //I think this is only for the HSE. it is called css(clock security system).
}

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

    clk::setSysClockFreq(25);
}

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
    clockConfiguration();
    PLLConfiguration();

    volatile int ahb_freq = clk::computeAHBFreq();
    volatile int apb1_freq = clk::computeAPB1Freq();
    volatile int apb2_freq = clk::computeAPB2Freq();

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


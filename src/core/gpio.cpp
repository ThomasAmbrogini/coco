/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

#include "common.h"
#include "temperature_humidity_sensor.h"
#include "timer.h"

#include <stdbool.h>

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    /**/
    LL_GPIO_ResetOutputPin(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin);

    /**/
    GPIO_InitStruct.Pin = TEMPERATURE_HUMIDITY_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(TEMPERATURE_HUMIDITY_GPIO_Port, &GPIO_InitStruct);

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
    EXTI->RTSR |= EXTI_RTSR_TR1;
    EXTI->FTSR |= EXTI_FTSR_TR1;
    NVIC_SetPriority(EXTI1_IRQn, 0);
    NVIC_EnableIRQ(EXTI1_IRQn);

    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void EXTI1_IRQHandler(void)
{
    uint32_t current_us = getCurrentUsTick();

    /* I have to be awaken on both rising and falling edge of the gpio. */
    bool gpio_value = LL_GPIO_IsOutputPinSet(GPIOA, LL_GPIO_PIN_1);

    temp::advanceStateMachine(gpio_value, current_us);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

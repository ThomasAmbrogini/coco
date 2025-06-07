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

    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
}

uint32_t falling_us_tick = 0;
uint32_t rising_us_tick  = 0;
uint32_t elapsed_us_tick = 0;

int num_rising_edges = 0;
int num_falling_edges = 0;

int first_edge = 1;
volatile int beauty_low[100] = {0};
volatile int beauty_high[100] = {0};
volatile int current_bit_low {};
volatile int current_bit_high {};

void __attribute__((used)) EXTI1_IRQHandler(void) {
//    if (first_edge) {
//        first_edge = false;
//    } else {
//        uint32_t current_us = getCurrentUsTick();
//
//        /* I have to be awaken on both rising and falling edge of the gpio. */
//        bool gpio_value = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_1);
//
//        if (gpio_value) {
//            ++num_rising_edges;
//        } else {
//            ++num_falling_edges;
//        }
//
//        temp::advanceStateMachine(gpio_value, current_us);
//    }
//
//    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
//
    if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_1)) {
        beauty_low[current_bit_low++] = getCurrentUsTick();
    } else {
        beauty_high[current_bit_high++] = getCurrentUsTick();
    }

    restartUsTick();
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
}


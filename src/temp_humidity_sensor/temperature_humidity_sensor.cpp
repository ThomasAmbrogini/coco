#include "temperature_humidity_sensor.h"

#include "common.h"
#include "gpio.h"
#include "timer.h"

#include "stm32f4xx_ll_utils.h"

namespace temp {

typedef enum {
    WAITING_START,
    START_HIGH,
    START_INFO_COMMUNICATION,
    WAIT_DATA,
    DECODE_DATA,

    ERROR
} ReadStates;

void init() {

}

void takeGpioAsOutput() {
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_GPIO_ResetOutputPin(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin);

  /**/
  GPIO_InitStruct.Pin = TEMPERATURE_HUMIDITY_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

  LL_GPIO_Init(TEMPERATURE_HUMIDITY_GPIO_Port, &GPIO_InitStruct);
}

void letGpioToSensor() {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

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
}

int waitForLowLevel() {
    int elapsed_time_us = 0;

    while(LL_GPIO_IsInputPinSet(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin)) {
        sleepUs(1);
        elapsed_time_us += 1;
    }

    return elapsed_time_us;
}

int timeHighLevel() {
    int elapsed_time_us = 0;

    while(LL_GPIO_IsInputPinSet(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin)) {
        sleepUs(1);
        elapsed_time_us += 1;
    }

    return elapsed_time_us;
}

int timeLowLevel() {
    int elapsed_time_us = 0;

    while(!LL_GPIO_IsInputPinSet(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin)) {
        sleepUs(1);
        elapsed_time_us += 1;
    }

    return elapsed_time_us;
}

void read() {
    constexpr int NUM_TOTAL_BITS = 5 * 8;
    volatile int buffer[NUM_TOTAL_BITS] = {0};

    takeGpioAsOutput();
    LL_GPIO_ResetOutputPin(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin);

    /* Start signal (line low) for at least 1 ms */
    /* maybe i can use the systick for this kind of delay */
    constexpr int start_delay_ms = 2;
    LL_mDelay(start_delay_ms);

    letGpioToSensor();

    /* Everything will be done in the EXTI1_IRQHandler */

//    /* Should be ~20-40 us */
//    int elapsed_time_us_after_start = waitForLowLevel();
//
//    /* Should be aruond ~80 us */
//    int elapsed_low_level = timeLowLevel();
//    int elapsed_high_level = timeHighLevel();
//
//    int count_bits = 0;
//    while(count_bits < NUM_TOTAL_BITS) {
//        /*
//         * every bit's transmission starts with low-voltage level that last 50
//         * us
//         */
//        int time_before_bit_value = timeLowLevel();
//
//        /*
//         * After the 50 us of low voltage, the duration of the next high signal
//         * determines the value of the bit.
//         * If the duration is ~26-28 us than it is low level, if the duration is
//         * ~70us than the value is high.
//         */
//        int time_for_bit = timeHighLevel();
//        int current_value = 0;
//        if (time_for_bit < 10) {
//            current_value = 0;
//        } else {
//            current_value = 1;
//        }
//
//        buffer[count_bits] = current_value;
//        ++count_bits;
//    }
//
//    buffer[0] = 0;
}

static ReadStates state;
static int start_operation_us = 0;
bool hum_temp_buffer[40] = {0};
int current_bit = 0;

void advanceStateMachine(bool value, uint32_t us) {
    switch (state) {
        case WAITING_START:
        {
            if (value)
            {
                state = ERROR;
            }
            else
            {
                current_bit = 0;
                start_operation_us = us;
                state = START_HIGH;
            }
        } break;

        case START_HIGH:
        {
            if (!value)
            {
                state = ERROR;
            }
            else
            {
                int us_passed = us - start_operation_us;

                if (us_passed >= 70 && us_passed <= 90)
                {
                    state = START_INFO_COMMUNICATION;
                    start_operation_us = us;
                }
                else
                {
                    state = ERROR;
                }
            }
        } break;

        case START_INFO_COMMUNICATION:
        {
            if (value)
            {
                state = ERROR;
            }
            else
            {
                int us_passed = us - start_operation_us;
                if (us_passed >= 70 && us_passed <= 90)
                {
                    start_operation_us = us;
                    state = DECODE_DATA;
                }
                else
                {
                    state = ERROR;
                }
            }
        } break;

        case DECODE_DATA:
        {
            if (!value)
            {
                state = ERROR;
            }
            else
            {
                int us_passed = us - start_operation_us;
                if (us_passed >= 20 && us_passed <= 30)
                {
                    state = WAIT_DATA;
                    hum_temp_buffer[current_bit++] = false;
                }
                else if (us_passed >= 30 && us_passed <= 80)
                {
                    state = WAIT_DATA;
                    hum_temp_buffer[current_bit++] = true;
                }
                else
                {
                    state = ERROR;
                }
                start_operation_us = us;

                if (current_bit == 40)
                {
                    state = WAITING_START;
                }
            }
        } break;

        case WAIT_DATA:
        {
            if (value)
            {
                state = ERROR;
            }
            else
            {
                int us_passed = us - start_operation_us;
                if (us_passed >= 40 && us_passed <= 60)
                {
                    state = DECODE_DATA;
                    start_operation_us = us;
                }
                else
                {
                    state = ERROR;
                }
            }
        } break;
    }
}

} /* namespace temp */


#include "temperature_humidity_sensor.h"

#include "common.h"
#include "gpio.h"
#include "timer.h"

#include "stm32f4xx_ll_utils.h"

#include <limits>

namespace temp {

typedef enum {
    WAITING_START = 0,
    START_HIGH,
    START_INFO_COMMUNICATION,
    WAIT_DATA,
    DECODE_DATA,

    ERROR
} read_states;

void init() {

}

void takeGpioAsOutput() {
  LL_GPIO_InitTypeDef GpioInitStruct = {0};

  LL_GPIO_ResetOutputPin(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin);

  /**/
  GpioInitStruct.Pin = TEMPERATURE_HUMIDITY_Pin;
  GpioInitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GpioInitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GpioInitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GpioInitStruct.Pull = LL_GPIO_PULL_NO;

  LL_GPIO_Init(TEMPERATURE_HUMIDITY_GPIO_Port, &GpioInitStruct);
}

void letGpioToSensor(bool InterruptMode) {
    LL_GPIO_InitTypeDef GpioInitStruct = {0};

    /**/
    LL_GPIO_ResetOutputPin(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin);

    GpioInitStruct.Pin = TEMPERATURE_HUMIDITY_Pin;
    GpioInitStruct.Mode = LL_GPIO_MODE_INPUT;
    GpioInitStruct.Pull = LL_GPIO_PULL_UP;

    if (InterruptMode) {
        SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
        NVIC_SetPriority(EXTI1_IRQn, 0);
        NVIC_EnableIRQ(EXTI1_IRQn);
        LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_1);
        LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_1);
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_1);
    }

    LL_GPIO_Init(TEMPERATURE_HUMIDITY_GPIO_Port, &GpioInitStruct);
}

uint32_t timeHighLevel() {
    uint32_t StartTicks = getCurrentUsTick();
    while(LL_GPIO_IsInputPinSet(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin)) {
    }
    uint32_t EndTicks = getCurrentUsTick();

    uint32_t Difference {EndTicks - StartTicks};
    if (EndTicks < StartTicks) {
        Difference = std::numeric_limits<uint32_t>::max() - StartTicks +
            EndTicks + 1;
    }

    return Difference;
}

int timeLowLevel() {
    uint32_t StartTicks = getCurrentUsTick();
    while(!LL_GPIO_IsInputPinSet(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin)) {
    }
    uint32_t EndTicks = getCurrentUsTick();

    uint32_t Difference {EndTicks - StartTicks};
    if (EndTicks < StartTicks) {
        Difference = std::numeric_limits<uint32_t>::max() - StartTicks +
            EndTicks + 1;
    }

    return Difference;
}

hum_temp_reading read() {
    constexpr bool INTERRUPT_MODE = false;
    constexpr int NUM_TOTAL_BITS = 5 * 8;

    uint16_t HumidityBits {};
    uint16_t TemperatureBits {};

    takeGpioAsOutput();
    LL_GPIO_ResetOutputPin(TEMPERATURE_HUMIDITY_GPIO_Port, TEMPERATURE_HUMIDITY_Pin);

    /* Start signal (line low) for at least 1 ms */
    /* maybe i can use the systick for this kind of delay */
    constexpr int StartDelayMs = 20;
    LL_mDelay(StartDelayMs);

    restartUsTick();
    letGpioToSensor(INTERRUPT_MODE);

    /* Should be ~20-40 us */
    int TimedUs = timeHighLevel();

    /* Should be aruond ~80 us */
    TimedUs = timeLowLevel();
    TimedUs = timeHighLevel();

    int CountBits {};
    while(CountBits < NUM_TOTAL_BITS) {
        /*
         * every bit's transmission starts with low-voltage level that last 50
         * us
         */
        int TimeBeforeBitValue = timeLowLevel();

        /*
         * After the 50 us of low voltage, the duration of the next high signal
         * determines the value of the bit.
         * If the duration is ~26-28 us than it is low level,
         * if the duration is ~70us than the value is high.
         */
        int TimeForBit = timeHighLevel();
        int CurrentValue = 0;
        if (TimeForBit < 40) {
            CurrentValue = 0;
        } else {
            CurrentValue = 1;
        }

        if (CountBits < 16) {
            HumidityBits |= (CurrentValue << (15 - CountBits));
        } else if (CountBits < 32) {
            TemperatureBits |= (CurrentValue << (15 - (CountBits % 16)));
        }
        ++CountBits;
    }

    constexpr uint32_t HIGH_BIT_HUMIDITY_MASK = 0x0000001100;
    float Humidity = HumidityBits / 10.0f;
    float Temperature = TemperatureBits / 10.0f;

    return hum_temp_reading { Humidity, Temperature };
}

static read_states State = WAITING_START;
static int StartOperationUs = 0;
bool HumTempBuffer[40] = {0};
int CurrentBit = 0;

void advanceStateMachine(bool Value, uint32_t Us) {
    switch (State) {
        case WAITING_START:
        {
            if (Value)
            {
                State = ERROR;
            }
            else
            {
                CurrentBit = 0;
                StartOperationUs = Us;
                State = START_HIGH;
            }
        } break;

        case START_HIGH:
        {
            if (!Value)
            {
                State = ERROR;
            }
            else
            {
                int UsPassed = Us - StartOperationUs;

                if (UsPassed >= 65 && UsPassed <= 90)
                {
                    State = START_INFO_COMMUNICATION;
                    StartOperationUs = Us;
                }
                else
                {
                    State = ERROR;
                }
            }
        } break;

        case START_INFO_COMMUNICATION:
        {
            if (Value)
            {
                State = ERROR;
            }
            else
            {
                int UsPassed = Us - StartOperationUs;
                if (UsPassed >= 70 && UsPassed <= 90)
                {
                    StartOperationUs = Us;
                    State = DECODE_DATA;
                }
                else
                {
                    State = ERROR;
                }
            }
        } break;

        case DECODE_DATA:
        {
            if (!Value)
            {
                State = ERROR;
            }
            else
            {
                int UsPassed = Us - StartOperationUs;
                if (UsPassed >= 20 && UsPassed <= 30)
                {
                    State = WAIT_DATA;
                    HumTempBuffer[CurrentBit++] = false;
                }
                else if (UsPassed >= 30 && UsPassed <= 80)
                {
                    State = WAIT_DATA;
                    HumTempBuffer[CurrentBit++] = true;
                }
                else
                {
                    State = ERROR;
                }
                StartOperationUs = Us;

                if (CurrentBit == 37) {
                    int C{};
                }
                if (CurrentBit == 40)
                {
                    State = WAITING_START;
                }
            }
        } break;

        case WAIT_DATA:
        {
            if (Value)
            {
                State = ERROR;
            }
            else
            {
                int UsPassed = Us - StartOperationUs;
                if (UsPassed >= 20 && UsPassed <= 60)
                {
                    State = DECODE_DATA;
                    StartOperationUs = Us;
                }
                else
                {
                    State = ERROR;
                }
            }
        } break;
    }
}

} /* namespace temp */


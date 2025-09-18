#pragma once

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"

namespace uart {

//TODO: i will start by doing everything for the USART2. This has to be expanded to all the others.
static USART_TypeDef* usart_reg = USART2;

static void gpio_pin_configuration();

//TODO: pass the instance as a template parameter.
void configuration() {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    gpio_pin_configuration();

    LL_USART_Enable(usart_reg);

    LL_USART_SetDataWidth(usart_reg, LL_USART_DATAWIDTH_8B);

    LL_USART_SetStopBitsLength(usart_reg, LL_USART_STOPBITS_1);

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    static constexpr float desired_baud_rate = 9600;
    /* The frequency of the clock is the one of the bus */
    //TODO: take the value from the clock header.
    static constexpr int freq_clock_hz = 50000000;
    static constexpr float usart_div = freq_clock_hz / (16 * desired_baud_rate);
    int integral_part = static_cast<int>(usart_div);
    int floating_part_fixed_point = (usart_div - integral_part) * 0x10;
    usart_reg->BRR = integral_part << 4 | floating_part_fixed_point;

    //TODO: why is the set of this bit atomic?
    LL_USART_EnableDirectionTx(usart_reg);
}

void write(char single_data) {
    while(!LL_USART_IsActiveFlag_TXE(usart_reg)) {
    }
    LL_USART_TransmitData8(usart_reg, single_data);
}

void write(const char * data, int size) {
    for (int i = 0; i < size; ++i) {
        write(data[i]);
    }
}

void end_transmission() {
    while (LL_USART_IsActiveFlag_TC(usart_reg)) {
    }
}

void gpio_pin_configuration() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);
}

} /* namespace uart */


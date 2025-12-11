#include "stm32f4xx_ll_rcc.h"

#include <stdint.h>

void NMI_Handler() {
    if (LL_RCC_IsActiveFlag_HSECSS()) {
        /* clear the interrupt flag otherwise the interrupt will be indefinitely called. */
        LL_RCC_ClearFlag_HSECSS();

        //TODO: log the event.

        //TODO: configure the clock with the PLL_HSI source.
    }
}

void HardFault_Handler() {
    volatile uint32_t* cfsr = &SCB->CFSR;
    volatile uint32_t* hfsr = &SCB->HFSR;
    while (1);
}


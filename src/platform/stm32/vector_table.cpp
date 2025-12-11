#include <stdint.h>

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx.h"

static void startup_handler();

int main();

using Isr = void (*)();

extern "C" void Default_Handler(void) {
    while (true) {}
}

void __attribute__((weak, alias("Default_Handler"))) EXTI0_IRQHandler(void);
void __attribute__((weak, alias("Default_Handler"))) EXTI1_IRQHandler(void);
void __attribute__((weak, alias("Default_Handler"))) NMI_Handler(void);
void __attribute__((weak, alias("Default_Handler"))) HardFault_Handler(void);
void __attribute__((weak, alias("Default_Handler"))) MemManage_Handler(void);
void __attribute__((weak, alias("Default_Handler"))) BusFault_Handler(void);
void __attribute__((weak, alias("Default_Handler"))) UsageFault_Handler(void);

__attribute__((section(".isr_vector"))) Isr interrupt_vector_table[] = {
    (Isr) 0x20004000,
     startup_handler,
     NMI_Handler,
     HardFault_Handler,
     MemManage_Handler,
     BusFault_Handler,
     UsageFault_Handler,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    (Isr) 0x0,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler
};

extern uint8_t __bss_start;
extern uint8_t __bss_end;

extern uint8_t __rom_data_start;
extern uint8_t __rom_data_end;
extern uint8_t __data_start;
extern uint8_t __data_end;

void startup_handler() {
    uint32_t * start_data = reinterpret_cast<uint32_t *>(&__data_start);
    uint32_t * end_data_rom = reinterpret_cast<uint32_t *>(&__rom_data_end);
    uint32_t * current_rom_data =
        reinterpret_cast<uint32_t *>(&__rom_data_start);

    while (current_rom_data != end_data_rom)
    {
        *start_data = *current_rom_data;
        ++start_data;
        ++current_rom_data;
    }

    uint32_t* current_bss_data = reinterpret_cast<uint32_t*>(&__bss_start);
    uint32_t* end_bss_data = reinterpret_cast<uint32_t*>(&__bss_end);

    while (current_bss_data != end_bss_data) {
        *current_bss_data = 0;
        ++current_bss_data;
    }

    main();
}

void NMI_Handler() {
    if (LL_RCC_IsActiveFlag_HSECSS()) {
        /* clear the interrupt flag otherwise the interrupt will be indefinitely called. */
        LL_RCC_ClearFlag_HSECSS();

        //TODO: log the event.

        //TODO: configure the clock with the PLL_HSI source.
    }
}

void HardFault_Handler(void) {
    volatile uint32_t* cfsr = &SCB->CFSR;
    volatile uint32_t* hfsr = &SCB->HFSR;
    while (1); // trap
}


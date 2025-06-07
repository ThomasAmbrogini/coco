#include <stdint.h>

static void startup_handler();

int main();

using Isr = void (*)();

extern "C" void Default_Handler(void) {
    while (true) {}
}

void __attribute__((weak, alias("Default_Handler"))) EXTI0_IRQHandler(void);
void __attribute__((weak, alias("Default_Handler"))) EXTI1_IRQHandler(void);

__attribute__((section(".isr_vector"))) Isr interrupt_vector_table[] = {
    (Isr) 0x20004000,
     startup_handler,
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


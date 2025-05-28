static void startup_handler();

int main();

using Isr = void (*)();

extern "C" void Default_Handler(void) {
    while (1) {}
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

void startup_handler() {
    int c = 4 + 5;
    main();
}


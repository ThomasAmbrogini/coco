static void startup_handler();

int main();

using Isr = void (*)();

__attribute__((section(".isr_vector"))) Isr interrupt_vector_table[] = {
    (Isr) 0x20004000,
    startup_handler
};

void startup_handler() {
    int c = 4 + 5;
    main();
}


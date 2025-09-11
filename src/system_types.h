#pragma once

namespace clk {

enum class Prescaler {
    div1 = 1,
    div2 = 2,
    div4 = 4,
    div8 = 8,
    div16 = 16,
    div64 = 64,
    div128 = 128,
    div256 = 256,
    div512 = 512,
};

struct PLLParams {
    int pllm;
    int plln;
    int pllp;
};

enum class Bus {
    AHB,
    APB1,
    APB2,
};

enum class ClockSource {
    HSI,
    HSE,
    PLL_HSI,
    PLL_HSE,
};

} /* namespace clk */

namespace tim {

//TODO: add the other timer instances.
enum class TimerInstance {
    Timer1,
};

}


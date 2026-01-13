#pragma once

namespace clk {

enum class prescaler {
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

struct pll_params {
    int Pllm;
    int Plln;
    int Pllp;
};

enum class bus {
    AHB,
    APB1,
    APB2,
};

enum class clock_source {
    HSI,
    HSE,
    PLL_HSI,
    PLL_HSE,
};

} /* namespace clk */

namespace tim {

//TODO: add the other timer instances.
enum class timer_instance {
    Timer1,
};

}


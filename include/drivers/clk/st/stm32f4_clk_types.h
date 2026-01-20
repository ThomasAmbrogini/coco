#pragma once

namespace clk {

enum class clock_source {
    HSI,
    HSE,
    PLL_HSI,
    PLL_HSE,
};

struct clock_tree {
    int SysclkFreqHz;
    int AHBFreqHz;
    int APB1FreqHz;
    int APB2FreqHz;
};

struct clock_config {
    int ExternalClockFreqHz;
    clock_tree ClockTree;
    clock_source ClockSource;
};

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

} /* namespace clk */


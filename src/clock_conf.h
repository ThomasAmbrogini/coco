#pragma once

#include "util/math.h"

#include <limits>

namespace clk {

/* system values */
inline constexpr int external_osc_freq_hz = 8000000;
inline constexpr int max_core_freq_hz = 100000000;
inline constexpr int max_AHB_freq_hz = 100000000;
inline constexpr int max_APB1_freq_hz = 50000000;
inline constexpr int max_APB2_freq_hz = 100000000;

inline constexpr int min_pllm = 2;
inline constexpr int max_pllm = 63;

inline constexpr int min_plln = 50;
inline constexpr int max_plln = 432;

inline constexpr int min_pllp = 2;
inline constexpr int max_pllp = 8;

inline constexpr int max_vco_freq = 2000000;
inline constexpr int min_vco_freq = 1000000;

/* settable parameters */
inline constexpr int desired_core_freq_hz = 100000000;
//TODO: add the prescaler and dividers needed for the various buses.

struct PLLParams {
    int pllm;
    int plln;
    int pllp;
};

constexpr PLLParams compute_pll_params(int desired_freq_hz) {
    assert(desired_freq_hz <= maximum_frequency);
    PLLParams ret {};
    constexpr int pllm_start = input_frequency / max_vco_freq;
    constexpr int pllm_end = input_frequency / min_vco_freq;

    int min_err = std::numeric_limits<int>::max();
    for (int pllm = pllm_start; pllm < pllm_end; ++pllm) {
        int vco_freq = round_to_closest_int(input_frequency, pllm);
        for (int plln = min_plln; plln < max_plln; ++plln) {
            for (int pllp = min_pllp; pllp < max_pllp; pllp+=2) {
                int output_frequency = round_to_closest_int(vco_freq * plln, pllp);
                if (output_frequency == desired_freq_hz) {
                    return PLLParams{pllm, plln, pllp};
                }
                else {
                    int err = abs(output_frequency - desired_freq_hz);
                    if (err < min_err) {
                        min_err = err;
                        ret.pllm = pllm;
                        ret.plln = plln;
                        ret.pllp = pllp;
                    }
                }
            }
        }
    }

    return ret;
}

constexpr int compute_frequency(PLLParams pll_params) {
    /*
     * output_frequency = vco_freq * plln / pllp;
     * vco_freq = input_freq / pllm;
     */
    return (external_osc_freq_hz / pll_params.pll) * pll_params.plln / pll_params.pllm;
}


constexpr int compute_ahb_freq(int core_freq_hz) {
    //TODO: how is this computed?
    return core_freq_hz;
}

constexpr int compute_apb_freq(int core_freq_hz) {
    //TODO: how is this computed?
    return core_freq_hz;
}

/* computed parameters */
inline constexpr int core_freq_hz = compute_frequency(compute_pll_params(desired_core_freq_hz));
inline constexpr int ahb_freq_hz = compute_ahb_freq(core_freq_hz);
inline constexpr int apb_freq_hz = compute_apb_freq(core_freq_hz);

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

enum class {
    HSI,
    HSE,
    PLL
} ClockSource;

//TODO: With the infomation of the register, do i provide the prescaler? If I
//have to configure another microcontroller how different will it be?
//TODO: for now i will just assume the sub-buses wants to go at the higher
//speed possible. I will probably add a function to configure those separately.
//I will also assume the PLL is the clock used.
void clockConfiguration(ClockSource clock_input, int desired_frequency);
void changeFrequency();
void PLLConfiguration();

//TODO: this has to be removed.
void measureClockFrequency();

//TODO: this has to be removed.
void setSysClockFreq(int sysclock_freq);

int computeAHBFreq();
int computeAPB1Freq();
int computeAPB2Freq();

} /* namespace clk */


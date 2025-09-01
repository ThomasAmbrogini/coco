#pragma once

#include "system_info.h"
#include "util/math.h"

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"

#include <limits>

namespace clk {

//TODO: I need a file which has all the params which are settable of the system.
/* settable parameters */
inline constexpr int desired_sysclk_freq_hz = 100000000;
//TODO: add the prescaler and dividers needed for the various buses.

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

inline constexpr Prescaler apb1_prescaler = Prescaler::div1;
inline constexpr Prescaler apb2_prescaler = Prescaler::div1;
inline constexpr Prescaler ahb_prescaler = Prescaler::div2;

struct PLLParams {
    int pllm;
    int plln;
    int pllp;
};

//TODO: all these constexpr functions can be moved to an util clock header.
constexpr PLLParams compute_pll_params(int desired_freq_hz) {
    //TODO: implement custom assert function. We need the UART to implement this.
    //assert(desired_freq_hz <= maximum_frequency);
    PLLParams ret {};
    constexpr int pllm_start = external_osc_freq_hz / max_vco_freq;
    constexpr int pllm_end = external_osc_freq_hz / min_vco_freq;

    int min_err = std::numeric_limits<int>::max();
    for (int pllm = pllm_start; pllm < pllm_end; ++pllm) {
        int vco_freq = round_to_closest_int(external_osc_freq_hz, pllm);
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
    return (external_osc_freq_hz / pll_params.pllm) * pll_params.plln / pll_params.pllp;
}

enum class Bus {
    AHB,
    APB1,
    APB2,
};

template<Bus _bus, Prescaler _prescaler>
constexpr u32 get_ahb_bus_prescaler_value() {
    static_assert(_prescaler <= Prescaler::div512, "AHB bus supports up to div512 prescaler");

    switch (_prescaler) {
        case Prescaler::div1: {
            return RCC_CFGR_HPRE_DIV1;
        } break;
        case Prescaler::div2: {
            return RCC_CFGR_HPRE_DIV2;
        } break;
        case Prescaler::div4: {
            return RCC_CFGR_HPRE_DIV4;
        } break;
        case Prescaler::div8: {
            return RCC_CFGR_HPRE_DIV8;
        } break;
        case Prescaler::div16: {
            return RCC_CFGR_HPRE_DIV16;
        } break;
        case Prescaler::div64: {
            return RCC_CFGR_HPRE_DIV64;
        } break;
        case Prescaler::div128: {
            return RCC_CFGR_HPRE_DIV128;
        } break;
        case Prescaler::div256: {
            return RCC_CFGR_HPRE_DIV256;
        } break;
        case Prescaler::div512: {
            return RCC_CFGR_HPRE_DIV256;
        } break;
        default: {
        } break;
    }
}

template<Bus _bus, Prescaler _prescaler>
constexpr u32 get_apb1_bus_prescaler_value() {
    static_assert(_prescaler <= Prescaler::div16, "APB1 bus supports up to div16 prescaler");

    switch (_prescaler) {
        case Prescaler::div1: {
            return RCC_CFGR_PPRE1_DIV1;
        } break;
        case Prescaler::div2: {
            return RCC_CFGR_PPRE1_DIV2;
        } break;
        case Prescaler::div4: {
            return RCC_CFGR_PPRE1_DIV4;
        } break;
        case Prescaler::div8: {
            return RCC_CFGR_PPRE1_DIV8;
        } break;
        case Prescaler::div16: {
            return RCC_CFGR_PPRE1_DIV16;
        } break;
        case Prescaler::div64:
        case Prescaler::div128:
        case Prescaler::div256:
        case Prescaler::div512:
        default: {
        } break;
    }
}

template<Bus _bus, Prescaler _prescaler>
constexpr u32 get_apb2_bus_prescaler_value() {
    static_assert(_prescaler <= Prescaler::div16, "APB2 bus supports up to div16 prescaler");

    switch (_prescaler) {
        case Prescaler::div1: {
            return RCC_CFGR_PPRE1_DIV1;
        } break;
        case Prescaler::div2: {
            return RCC_CFGR_PPRE1_DIV2;
        } break;
        case Prescaler::div4: {
            return RCC_CFGR_PPRE1_DIV4;
        } break;
        case Prescaler::div8: {
            return RCC_CFGR_PPRE1_DIV8;
        } break;
        case Prescaler::div16: {
            return RCC_CFGR_PPRE1_DIV16;
        } break;
        case Prescaler::div64:
        case Prescaler::div128:
        case Prescaler::div256:
        case Prescaler::div512:
        default: {
        } break;
    }
}

template<Bus _bus, Prescaler _prescaler>
constexpr u32 convert_prescaler() {
    if constexpr (_bus == Bus::AHB) {
        return get_ahb_bus_prescaler_value<_bus, _prescaler>();
    } else if constexpr (_bus == Bus::APB1) {
        return get_apb1_bus_prescaler_value<_bus, _prescaler>();
    } else if constexpr (_bus == Bus::APB2) {
        return get_apb2_bus_prescaler_value<_bus, _prescaler>();
    }
}

template<Bus _bus, Prescaler _prescaler>
constexpr int compute_bus_freq(int sysclk_freq_hz) {
    //u32 prescaler_value = convert_prescaler<_bus, _prescaler>();
    return sysclk_freq_hz / static_cast<int>(_prescaler);
}

/* computed parameters */
inline constexpr int sysclk_freq_hz = compute_frequency(compute_pll_params(desired_sysclk_freq_hz));
inline constexpr int ahb_freq_hz = compute_bus_freq<Bus::AHB, ahb_prescaler>(sysclk_freq_hz);
inline constexpr int apb1_freq_hz = compute_bus_freq<Bus::APB1, apb1_prescaler>(sysclk_freq_hz);
inline constexpr int apb2_freq_hz = compute_bus_freq<Bus::APB2, apb2_prescaler>(sysclk_freq_hz);

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

enum class ClockSource {
    HSI,
    HSE,
    PLL
};

static inline void configureHSE() {
    /* hse_ready is not placed to true until the HSE is not turned on. */
    if (!LL_RCC_HSE_IsOn()) {
        LL_RCC_HSE_Enable();
        /*
         * I can switch to a clock only when it is read, but it seems that even if
         * i do it before it is actually ready, it will wait until it is ready before
         * changing the system clock to the specified source.
         */
        while(!LL_RCC_HSE_IsReady()) {
        }

        /* hse on is true in here. */
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
    {
    }
}

static inline void configureHSI() {
    if (!LL_RCC_HSI_IsReady()) {
        LL_RCC_HSI_Enable();
        while (!LL_RCC_HSI_IsReady()) {
        }
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
    {
    }
}

static inline void configurePLL() {
    if (!LL_RCC_PLL_IsReady()) {
        LL_RCC_PLL_Enable();
        while (!LL_RCC_PLL_IsReady()) {
        }
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }
}

template<ClockSource _clock_source, int _desired_frequency>
void clockConfiguration() {
    /**
     *
     * The system clock can be output on a pin with the MCO2 pin.
     *
     */
    //TODO(important): Do i need to power the RCC? */
    //TODO: flash latenycy computation. It has to be change before in case of
    //aceleration and after the clock change when decelarating.
    //TODO: Change the number of wait states based on the new frequency.
    //It also depends on the supplied power voltage which seems to be something
    //which is known based on the board.
    //static constexpr float SUPPLIED_POWER_VOLTAGE = 3.3f;
    //TODO: acelerating the clock frequeuncy.
    //TODO: it seems possible to compute the supplied power voltage also at
    //runtime, with the following steps
    // Example using ADC to read VREFINT
    // 1. Enable VREFINT channel
    // 2. Read ADC value
    // 3. Compute VDD using formula from the datasheet
    //VDD = VREFINT_CAL * 3.3 / ADC_READ_VALUE
    //If decelarating the clock frequency.
    //TODO: the number of wait states is probably going to be the last thing.

    //All of these can not be done at runtime, but i can compute all the configuration i need at compile time.
    /* The stm32f4 discovery has the X2 on-board oscillator which can be used for external.*/
    if constexpr (_clock_source == ClockSource::HSE) {
        configureHSE();
    } else if constexpr (_clock_source == ClockSource::HSI) {
        configureHSI();
    } else if constexpr (_clock_source == ClockSource::PLL) {
        //TODO: A lot of parameters are passed in here! How do i pass those ones?
        //TODO: this source clock is assumed to be the HSE. Later we will try to
        //add the support for other things.
        constexpr PLLParams params = compute_pll_params(_desired_frequency);
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, params.pllm, params.plln, params.pllp);

        configurePLL();
    }

    //TODO: i need to compute the prescaler
    //TODO: how do i pass those?
    //Do i need to do this before changing the clock? I think so, otherwise I
    //would have a different clock on the bus (which could be higher than the
    //maximum).

    constexpr u32 ahb_prescaler = convert_prescaler<Bus::AHB, Prescaler::div1>();
    constexpr u32 apb1_prescaler = convert_prescaler<Bus::APB1, Prescaler::div1>();
    constexpr u32 apb2_prescaler = convert_prescaler<Bus::APB1, Prescaler::div1>();

    LL_RCC_SetAHBPrescaler(ahb_prescaler);
    LL_RCC_SetAPB1Prescaler(apb1_prescaler);
    LL_RCC_SetAPB2Prescaler(apb2_prescaler);

    //TODO: how do i Configure this? What does it actually do?
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);

    //TODO: disable the current clock to save some power.
    //TODO: is there a case where you should have two clock sources, what about
    //the feature to fallback in case the HSE fails?
    //TODO: if i am using the pll with HSE as source, and the HSE fails, can i
    //use the PLL with the HSI?
    volatile u32 hsi_on = LL_RCC_HSI_IsOn();
    if (LL_RCC_HSI_IsOn()) {
        LL_RCC_HSI_Disable();
    }

    //TODO: i can now which clock is current used as system clock in the cr.

    //TODO: there is a safety mechanisms which says something when the clock fails.
    //I think this is only for the HSE. it is called css(clock security system).
}

} /* namespace clk */


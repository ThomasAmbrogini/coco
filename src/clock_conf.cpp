#include "clock_conf.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"

//TODO: algo to implement to find the closest clock to the desired one.
//#include <cassert>
//#include <fmt/base.h>
//#include <limits>
//
//inline constexpr int maximum_frequency = 100000000;
//inline constexpr int input_frequency = 8000000;
//
//struct PLLParams {
//    int pllm;
//    int plln;
//    int pllp;
//};
//
//
//template<typename T>
//constexpr T abs(T value) {
//    if (value < 0) {
//        return -value;
//    } else {
//        return value;
//    }
//}
//
//constexpr PLLParams compute_pll_params(int desired_frequency) {
//    assert(desired_frequency <= maximum_frequency);
//    PLLParams ret {};
//    /*
//     * output_frequency = vco_freq * plln / pllp;
//     * vco_freq = input_freq / pllm;
//     */
//    constexpr int min_pllm = 2;
//    constexpr int max_pllm = 63;
//
//    constexpr int min_plln = 50;
//    constexpr int max_plln = 432;
//
//    constexpr int min_pllp = 2;
//    constexpr int max_pllp = 8;
//
//    constexpr int max_vco_freq = 2000000;
//    constexpr int min_vco_freq = 1000000;
//
//    constexpr int pllm_start = input_frequency / max_vco_freq;
//    constexpr int pllm_end = input_frequency / min_vco_freq;
//
//    int min_err = std::numeric_limits<int>::max();
//    for (int pllm = pllm_start; pllm < pllm_end; ++pllm) {
//        int vco_freq = round_to_closest_int(input_frequency, pllm);
//        for (int plln = min_plln; plln < max_plln; ++plln) {
//            for (int pllp = min_pllp; pllp < max_pllp; pllp+=2) {
//                int output_frequency = round_to_closest_int(vco_freq * plln, pllp);
//                if (output_frequency == desired_frequency) {
//                    return PLLParams{pllm, plln, pllp};
//                }
//                else {
//                    int err = abs(output_frequency - desired_frequency);
//                    if (err < min_err) {
//                        min_err = err;
//                        ret.pllm = pllm;
//                        ret.plln = plln;
//                        ret.pllp = pllp;
//                    }
//                }
//            }
//        }
//    }
//
//    return ret;
//}
//
//int main() {
//    static constexpr int desired_frequency = 99999998;
//    constexpr PLLParams params = compute_pll_params(desired_frequency);
//
//    fmt::print("{},{},{}\n", params.pllm, params.plln, params.pllp);
//}


//TODO: maybe everything can be done in constexpr because the clock
//configuration will not be done at runtime.
namespace clk {

namespace stm32f4 {
    //TODO: what should this return?
    constexpr int computeFlashLatency(int frequencyMHz, float supplied_power_voltage);
} /* namespace stm32f4 */

int SYSClock_freq {};

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

struct PLLParams {
    int pllm;
    int plln;
    int pllp;
};

constexpr PLLParams computePLLParams(int desired_frequency) {
    /**
     * Limitations for the value of pllm:
     * Caution: The software has to set these bits correctly to ensure that the
     * VCO input frequency ranges from 1 to 2 MHz. It is recommended to select
     * a frequency of 2 MHz to limit PLL jitter.
     * VCO input frequency = PLL input clock frequency / PLLM with 2 ≤PLLM ≤63
     */
    static constexpr int ideal_vco_freq_MHz = 2;
    static constexpr int pllm = external_oscillator_freq_MHz  / ideal_vco_freq_MHz;
    static_assert(pllm >= 2 && pllm <= 63, "The pllm value is out of range");

    /**
     * Limitations for the value of plln:
     * Caution: The software has to set these bits correctly to ensure that the
     * VCO output frequency is between 100 and 432 MHz
     * VCO output frequency = VCO input frequency × PLLN with 50 ≤PLLN ≤432
     */
    static constexpr int plln = ;

    /**
     * Limitations for the value of pllp:
     * Caution: The software has to set these bits correctly not to exceed
     * 100 MHz on this domain.
     * PLL output clock frequency = VCO frequency / PLLP
     * with PLLP = 2, 4, 6, or 8
     */
    static constexpr int pllp = ;

    return PLLParams {.pllm = pllm, };
}

void clockConfiguration(ClockSource clock_input, int desired_frequency) {
    /**
     *
     * The system clock can be output on a pin with the MCO2 pin.
     *
     */

    //TODO(important): Do i need to power the RCC? */
    //TODO: flash latenycy computation. It has to be change before in case of
    //aceleration and after the clock change when decelarating.

    /* The stm32f4 discovery has the X2 on-board oscillator which can be used for external.*/
    if (clock_source == ClockSource::HSE) {
        configureHSE();
    } else if (clock_source == ClockSource::HSI) {
        configureHSI();
    } else if (clock_source == ClockSource::PLL) {
        //TODO: A lot of parameters are passed in here! How do i pass those ones?
        //TODO: this source clock is assumed to be the HSE. Later we will try to
        //add the support for other things.
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 100, LL_RCC_PLLP_DIV_2);

        configurePLL();
    }

    //TODO: how do i pass those?
    //Do i need to do this before changing the clock? I think so, otherwise I
    //would have a different clock on the bus (which could be higher than the
    //maximum).
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);

    //TODO: disable the current clock to save some power.
    //TODO: is there a case where you should have two clock sources, what about
    //the feature to fallback in case the HSE fails?
    //TODO: if i am using the pll with HSE as source, and the HSE fails, can i
    //use the PLL with the HSI?
    volatile uint32_t hsi_on = LL_RCC_HSI_IsOn();
    if (LL_RCC_HSI_IsOn()) {
        LL_RCC_HSI_Disable();
    }

    //TODO: i can now which clock is current used as system clock in the cr.

    //TODO: there is a safety mechanisms which says something when the clock fails.
    //I think this is only for the HSE. it is called css(clock security system).
}

void PLLConfiguration() {
    // PLL Configuration
    // PLL configuration can not change once it is enabled, so it has to be
    // configured before enabling it.
    //
    //
    // The software has to set these bits correctly to ensure that the VCO input frequency
    // ranges from 1 to 2 MHz. It is recommended to select a frequency of 2 MHz to limit
    // PLL jitter.

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 100, LL_RCC_PLLP_DIV_2);

    LL_RCC_PLL_Enable();
    while (!LL_RCC_PLL_IsReady()) {
    }

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    LL_SetSystemCoreClock(100000000);
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);

    clk::setSysClockFreq(25);
}

void changeFrequency() {
    //TODO: acelerating the clock frequeuncy.
    //TODO: Change the number of wait states based on the new frequency.
    //It also depends on the supplied power voltage which seems to be something
    //which is known based on the board.
    static constexpr float SUPPLIED_POWER_VOLTAGE = 3.3f;

    //TODO: it seems possible to compute the supplied power voltage also at
    //runtime, with the following steps
    // Example using ADC to read VREFINT
    // 1. Enable VREFINT channel
    // 2. Read ADC value
    // 3. Compute VDD using formula from the datasheet
    //VDD = VREFINT_CAL * 3.3 / ADC_READ_VALUE

    //If decelarating the clock frequency.
    //TODO: the number of wait states is probably going to be the last thing.
}

//TODO: this has to be removed.
void measureClockFrequency() {
    //TODO: i can also measure the frequency of the various clocks using two
    //different timers.

    /*
     * The desired clock source is selected using the MCO1PRE[2:0] and MCO1[1:0]
     * bits in the RCC clock configuration register (RCC_CFGR).
     */
    //MCO1 -> PA8
    //MCO2 -> PC9

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

    LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_9, LL_GPIO_AF_0);

    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);

    LL_RCC_ConfigMCO(LL_RCC_MCO2SOURCE_PLLCLK, LL_RCC_MCO2_DIV_1);
}

void setSysClockFreq(int sysclock_freq) {
    SYSClock_freq = sysclock_freq;
}

int computeAHBFreq() {
    //TODO: will i have the sysclock_freq as a global (or static) var or will i
    //pass it to the function itself?
    //I think I want to know the different values, so I am going to store it as
    //global var.
    return __LL_RCC_CALC_HCLK_FREQ(SYSClock_freq, LL_RCC_GetAHBPrescaler());
}

int computeAPB1Freq() {
    return __LL_RCC_CALC_PCLK1_FREQ(SYSClock_freq, LL_RCC_GetAPB1Prescaler());
}

int computeAPB2Freq() {
    return __LL_RCC_CALC_PCLK2_FREQ(SYSClock_freq, LL_RCC_GetAPB2Prescaler());
}

} /* namespace clk */


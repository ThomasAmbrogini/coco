#pragma once

//TODO: this should not be in here.
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using i8 = signed char;
using i16 = signed short;
using i32 = int;

namespace clk {

inline constexpr int external_osc_freq_hz = 8000000;
inline constexpr int internal_osc_freq_hz = 16000000;

inline constexpr int starting_core_freq_hz = internal_osc_freq_hz;
inline constexpr int max_core_freq_hz      = 100000000;
inline constexpr int max_AHB_freq_hz       = 100000000;
inline constexpr int max_APB1_freq_hz      = 50000000;
inline constexpr int max_APB2_freq_hz      = 100000000;

inline constexpr int min_pllm = 2;
inline constexpr int max_pllm = 63;

inline constexpr int min_plln = 50;
inline constexpr int max_plln = 432;

inline constexpr int min_pllp = 2;
inline constexpr int max_pllp = 8;

inline constexpr int min_vco_freq = 1000000;
inline constexpr int max_vco_freq = 2000000;

}/* namespace clk */


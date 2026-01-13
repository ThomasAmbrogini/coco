#pragma once

namespace clk {

inline constexpr int ExternalOscFreqHz = 8000000;
inline constexpr int InternalOscFreqHz = 16000000;

inline constexpr int StartingCoreFreqHz = InternalOscFreqHz;
inline constexpr int MaxCoreFreqHz      = 100000000;
inline constexpr int MaxAHBFreqHz       = 100000000;
inline constexpr int MaxAPB1FreqHz      = 50000000;
inline constexpr int MaxAPB2FreqHz      = 100000000;

inline constexpr int MinPLLM = 2;
inline constexpr int MaxPLLM = 63;

inline constexpr int MinPLLN = 50;
inline constexpr int MaxPLLN = 432;

inline constexpr int MinPLLP = 2;
inline constexpr int MaxPLLP = 8;

inline constexpr int MinVCOFreq = 1000000;
inline constexpr int MaxVCOFreq = 2000000;

}/* namespace clk */


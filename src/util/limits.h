#pragma once

enum class value_type {
    Unsigned,
    Signed,
};

template<value_type _ValueType, int _NumBits>
class bit_limits {
public:
    static constexpr int max() {
        if constexpr (IsSigned_) {
            return (1 << (_NumBits - 1)) - 1;
        } else {
            return (1 << (_NumBits)) - 1;
        }
    }

private:
    static constexpr bool IsSigned_ { _ValueType == value_type::Signed };
};

template<int _NumBits>
using unsigned_bit_limits = bit_limits<value_type::Unsigned, _NumBits>;

static_assert(bit_limits<value_type::Unsigned, 12>::max() == 4095);
static_assert(unsigned_bit_limits<12>::max() == 4095);


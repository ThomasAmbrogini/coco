#pragma once

enum class ValueType {
    Unsigned,
    Signed,
};

template<ValueType _value_type, int _num_bits>
class bit_limits {
public:
    static constexpr int max() {
        if constexpr (is_signed) {
            return (1 << (_num_bits - 1)) - 1;
        } else {
            return (1 << (_num_bits)) - 1;
        }
    }

private:
    static constexpr bool is_signed { _value_type == ValueType::Signed };
};

template<int _num_bits>
using unsigned_bit_limits = bit_limits<ValueType::Unsigned, _num_bits>;

static_assert(bit_limits<ValueType::Unsigned, 12>::max() == 4095);
static_assert(unsigned_bit_limits<12>::max() == 4095);


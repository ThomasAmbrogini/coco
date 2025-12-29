#pragma once

#include "assert.h"
#include "ros/string_view.h"
#include "ros/types.h"

#include "time/time.h"

//TODO: remove once the memcpy has been removed
#include <string.h>

namespace print {

template<uint32_t _size>
struct DataRingBuffer {
    char data[_size];
    int head_lpos;
    int tail_lpos;
};

struct Descriptor {
    int start_lpos;
    int next_lpos;
};

template<uint32_t _size>
struct DescRingBuffer {
    Descriptor desc[_size];
    int head_id;
    int tail_id;
};

template<uint32_t _data_size, uint32_t _desc_size>
struct RingBuffer {
    DataRingBuffer<_data_size> data_ring;
    DescRingBuffer<_desc_size> desc_ring;
    static constexpr auto data_size {_data_size};
    static constexpr auto desc_size {_desc_size};
};

constexpr bool wraps(int begin_lpos, int len, int size) {
    int next_lpos = (begin_lpos % size) + len;
    return (next_lpos > size);
}

static_assert(wraps(0, 10, 20) == false);
static_assert(wraps(15, 10, 20) == true);
static_assert(wraps(0, 10, 10) == false);

constexpr int get_begin_lpos(const int begin_lpos, const int len, const int size) {
    if (wraps(begin_lpos, len, size)) {
        const int supposed_next_lpos = begin_lpos + len;
        return supposed_next_lpos - (supposed_next_lpos % size);
    } else {
        return begin_lpos;
    }
}

static_assert(get_begin_lpos(0, 10, 20) == 0);
static_assert(get_begin_lpos(55, 10, 20) == 60);
static_assert(get_begin_lpos(45, 10, 20) == 45);
static_assert(get_begin_lpos(9, 1, 10) == 9);

constexpr int get_next_lpos(const int begin_lpos, const int len, const int size) {
    if (wraps(begin_lpos, len, size)) {
        const int supposed_next_lpos = begin_lpos + len;
        return supposed_next_lpos - (supposed_next_lpos % size) + len;
    } else {
        return begin_lpos + len;
    }
}

static_assert(get_next_lpos(0, 10, 20) == 10);
static_assert(get_next_lpos(10, 10, 20) == 20);
static_assert(get_next_lpos(20, 10, 20) == 30);

constexpr int modulo_idx(int counter, int size) {
    return counter % size;
}

static_assert(modulo_idx(5, 10) == 5);
static_assert(modulo_idx(15, 10) == 5);
static_assert(modulo_idx(10, 10) == 0);
static_assert(modulo_idx(-1, 10) == -1);

volatile uint32_t sum_stores = 0;
volatile uint32_t count_stores  = 0;

/**
 * @brief stores a log inside the ring buffer.
 * @details
 *    If a msg which does not fit inside the buffer is given, the function will
 *    just return.
 */
template<int _data_size, int _desc_size>
constexpr int store_log(RingBuffer<_data_size, _desc_size>& rb, ros::StringView data) {
    static_assert(_data_size > 0);
    static_assert(_desc_size > 0);
    assert(data.data() != nullptr);
    assert(data.size() != 0);

    volatile uint32_t start_tick = TIM2->CNT;

    auto& data_ring {rb.data_ring};
    auto& desc_ring {rb.desc_ring};

    const int free_bytes {_data_size - (data_ring.tail_lpos - data_ring.head_lpos)};
    const int num_free_desc {_desc_size - (desc_ring.tail_id - desc_ring.head_id)};

    if (free_bytes < data.size()) {
        return ERROR;
    } else if (num_free_desc == 0) {
        return ERROR;
    }

    const int data_tail {data_ring.tail_lpos};
    const int begin_lpos {get_begin_lpos(data_tail, data.size(), rb.data_size)};
    const int next_lpos {get_next_lpos(data_tail, data.size(), rb.data_size)};
    data_ring.tail_lpos = next_lpos;

    const int desc_idx {desc_ring.tail_id};
    const Descriptor desc {.start_lpos {begin_lpos}, .next_lpos {next_lpos}};
    desc_ring.desc[modulo_idx(desc_idx, rb.desc_size)] = desc;
    ++desc_ring.tail_id;

    memcpy(&data_ring.data[modulo_idx(begin_lpos, rb.data_size)], data.data(), data.size());

    volatile uint32_t end_tick = TIM2->CNT;

    uint32_t diff_tick;
    if (end_tick> start_tick) {
        diff_tick = end_tick - start_tick;
    } else {
        diff_tick = 0xFFFFFFFF - start_tick + end_tick;
    }

    sum_stores += diff_tick;
    count_stores += 1;

    return SUCCESS;
}

volatile uint32_t sum_retrieves {0};
volatile uint32_t count_retrieves {0};

/**
 * @brief get the next message from the ring buffer.
 * @return empty StringView if there were no messages, a StringView to the
 *         message otherwise.
 */
template<int _data_size, int _desc_size>
ros::StringView retrieve_log(RingBuffer<_data_size, _desc_size>& rb) {
    static_assert(_data_size > 0);
    static_assert(_desc_size > 0);

    volatile uint32_t start_tick = TIM2->CNT;

    auto& data_ring {rb.data_ring};
    auto& desc_ring {rb.desc_ring};

    if (desc_ring.head_id == desc_ring.tail_id) {
        return ros::StringView{};
    }

    const int desc_idx {desc_ring.head_id};
    const Descriptor desc {desc_ring.desc[modulo_idx(desc_idx, rb.desc_size)]};
    const int begin_lpos {desc.start_lpos};
    const int next_lpos {desc.next_lpos};
    const int len {next_lpos - begin_lpos};

    data_ring.head_lpos = next_lpos;
    ++desc_ring.head_id;
    ros::StringView ret {&data_ring.data[modulo_idx(begin_lpos, rb.data_size)], len};

    volatile uint32_t end_tick = TIM2->CNT;

    uint32_t diff_tick;
    if (end_tick > start_tick) {
        diff_tick = end_tick - start_tick;
    } else {
        diff_tick = 0xFFFFFFFF - start_tick + end_tick;
    }

    sum_retrieves += diff_tick;
    count_retrieves += 1;

    return ret;
}

} /* namespace log */


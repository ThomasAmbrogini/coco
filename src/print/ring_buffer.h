#pragma once

#include "assert.h"
#include "coco/string_view.h"
#include "coco/types.h"

#include "time/time.h"

//TODO: remove once the memcpy has been removed
#include <string.h>

namespace print {

template<uint32_t _Size>
struct data_ring_buffer {
    char Data[_Size];
    int HeadLpos;
    int TailLpos;
};

struct descriptor {
    int StartLpos;
    int NextLpos;
};

template<uint32_t _Size>
struct desc_ring_buffer {
    descriptor Desc[_Size];
    int HeadId;
    int TailId;
};

template<uint32_t _DataSize, uint32_t _DescSize>
struct ring_buffer {
    data_ring_buffer<_DataSize> DataRing;
    desc_ring_buffer<_DescSize> DescRing;
    static constexpr auto DataSize {_DataSize};
    static constexpr auto DescSize {_DescSize};
};

constexpr bool wraps(int BeginLpos, int Len, int Size) {
    int NextLpos = (BeginLpos % Size) + Len;
    return (NextLpos > Size);
}

static_assert(wraps(0, 10, 20) == false);
static_assert(wraps(15, 10, 20) == true);
static_assert(wraps(0, 10, 10) == false);

constexpr int get_begin_lpos(const int BeginLpos, const int Len, const int Size) {
    if (wraps(BeginLpos, Len, Size)) {
        const int SupposedNextLpos = BeginLpos + Len;
        return SupposedNextLpos - (SupposedNextLpos % Size);
    } else {
        return BeginLpos;
    }
}

static_assert(get_begin_lpos(0, 10, 20) == 0);
static_assert(get_begin_lpos(55, 10, 20) == 60);
static_assert(get_begin_lpos(45, 10, 20) == 45);
static_assert(get_begin_lpos(9, 1, 10) == 9);

constexpr int get_next_lpos(const int BeginLpos, const int Len, const int Size) {
    if (wraps(BeginLpos, Len, Size)) {
        const int SupposedNextLpos = BeginLpos + Len;
        return SupposedNextLpos - (SupposedNextLpos % Size) + Len;
    } else {
        return BeginLpos + Len;
    }
}

static_assert(get_next_lpos(0, 10, 20) == 10);
static_assert(get_next_lpos(10, 10, 20) == 20);
static_assert(get_next_lpos(20, 10, 20) == 30);

constexpr int modulo_idx(int Counter, int Size) {
    return Counter % Size;
}

static_assert(modulo_idx(5, 10) == 5);
static_assert(modulo_idx(15, 10) == 5);
static_assert(modulo_idx(10, 10) == 0);
static_assert(modulo_idx(-1, 10) == -1);

volatile uint32_t SumStores = 0;
volatile uint32_t CountStores  = 0;

/**
 * @brief stores a log inside the ring buffer.
 * @details
 *    If a msg which does not fit inside the buffer is given, the function will
 *    just return.
 */
template<int _DataSize, int _DescSize>
constexpr int store_log(ring_buffer<_DataSize, _DescSize>& Rb, coco::string_view Data) {
    static_assert(_DataSize > 0);
    static_assert(_DescSize > 0);
    assert(Data.data() != nullptr);
    assert(Data.size() != 0);

    __asm volatile ("" ::: "memory");
    volatile uint32_t StartTick = TIM2->CNT;
    __asm volatile ("" ::: "memory");

    auto& DataRing {Rb.DataRing};
    auto& DescRing {Rb.DescRing};

    const int FreeBytes {_DataSize - (DataRing.TailLpos - DataRing.HeadLpos)};
    const int NumFreeDesc {_DescSize - (DescRing.TailId - DescRing.HeadId)};

    if (FreeBytes < Data.size()) {
        return ERROR;
    } else if (NumFreeDesc == 0) {
        return ERROR;
    }

    const int DataTail {DataRing.TailLpos};
    const int BeginLpos {get_begin_lpos(DataTail, Data.size(), Rb.DataSize)};
    const int NextLpos {get_next_lpos(DataTail, Data.size(), Rb.DataSize)};
    DataRing.TailLpos = NextLpos;

    const int DescIdx {DescRing.TailId};
    const descriptor Desc {.StartLpos {BeginLpos}, .NextLpos {NextLpos}};
    DescRing.Desc[modulo_idx(DescIdx, Rb.DescSize)] = Desc;
    ++DescRing.TailId;

    memcpy(&DataRing.Data[modulo_idx(BeginLpos, Rb.DataSize)], Data.data(), Data.size());

    __asm volatile ("" ::: "memory");
    volatile uint32_t EndTick = TIM2->CNT;
    __asm volatile ("" ::: "memory");

    uint32_t DiffTick;
    if (EndTick> StartTick) {
        DiffTick = EndTick - StartTick;
    } else {
        DiffTick = 0xFFFFFFFF - StartTick + EndTick;
    }

    SumStores += DiffTick;
    CountStores += 1;

    return SUCCESS;
}

volatile uint32_t SumRetrieves {0};
volatile uint32_t CountRetrieves {0};

/**
 * @brief get the next message from the ring buffer.
 * @return empty string_view if there were no messages, a string_view to the
 *         message otherwise.
 */
template<int _DataSize, int _DescSize>
coco::string_view retrieve_log(ring_buffer<_DataSize, _DescSize>& Rb) {
    static_assert(_DataSize > 0);
    static_assert(_DescSize > 0);

    __asm volatile ("" ::: "memory");
    volatile uint32_t StartTick = TIM2->CNT;
    __asm volatile ("" ::: "memory");

    auto& DataRing {Rb.DataRing};
    auto& DescRing {Rb.DescRing};

    if (DescRing.HeadId == DescRing.TailId) {
        return coco::string_view{};
    }

    const int DescIdx {DescRing.HeadId};
    const descriptor Desc {DescRing.Desc[modulo_idx(DescIdx, Rb.DescSize)]};
    const int BeginLpos {Desc.StartLpos};
    const int NextLpos {Desc.NextLpos};
    const int Len {NextLpos - BeginLpos};

    DataRing.HeadLpos = NextLpos;
    ++DescRing.HeadId;
    coco::string_view Ret {&DataRing.Data[modulo_idx(BeginLpos, Rb.DataSize)], Len};

    __asm volatile ("" ::: "memory");
    volatile uint32_t EndTick = TIM2->CNT;
    __asm volatile ("" ::: "memory");

    uint32_t DiffTick;
    if (EndTick > StartTick) {
        DiffTick = EndTick - StartTick;
    } else {
        DiffTick = 0xFFFFFFFF - StartTick + EndTick;
    }

    SumRetrieves += DiffTick;
    CountRetrieves += 1;

    return Ret;
}

} /* namespace print */


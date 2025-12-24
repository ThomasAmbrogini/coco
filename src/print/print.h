#pragma once

#include "log/log.h"
#include "print/sink.h"
#include "ros/view.h"

namespace {
    inline constexpr int print_data_size {10 * 1024};
    inline constexpr int print_desc_size {64};
    using PrintRingBuffer = log::RingBuffer<print_data_size, print_desc_size>;
    PrintRingBuffer s_prb;

    Sink s_sink {};

    inline constexpr auto store_debug = log::store_log<log::Level::Debug, print_data_size, print_desc_size>;
    inline constexpr auto store_info  = log::store_log<log::Level::Info, print_data_size, print_desc_size>;
    inline constexpr auto store_error = log::store_log<log::Level::Error, print_data_size, print_desc_size>;
}

namespace print {

void register_sink(Sink sink) {
    s_sink = sink;
}

} /* namespace print */

void printr(const char* msg, int size) {
    ros::View<char> data_view {msg, size};
    store_info(s_prb, data_view);

    //TODO: i do not like this check.
    auto& desc_ring {s_prb.desc_ring};
    while(desc_ring.head_id != desc_ring.tail_id) {
        const log::DataBlk data_blk {log::retrieve_log<print_data_size, print_desc_size>(s_prb)};
        if (s_sink.valid)
        {
            s_sink.write(data_blk.data, data_blk.len);
        }
    }
}

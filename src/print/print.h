#pragma once

#include "log/log.h"
#include "print/sink.h"
#include "ros/string_view.h"

namespace {
    inline constexpr int print_data_size {10 * 1024};
    inline constexpr int print_desc_size {64};
    using PrintRingBuffer = log::RingBuffer<print_data_size, print_desc_size>;
    PrintRingBuffer s_prb {};

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

inline void printr(ros::StringView msg) {
    //TODO: this could be checked
    store_info(s_prb, msg);

    //TODO: i do not like this check.
    auto& desc_ring {s_prb.desc_ring};
    while(desc_ring.head_id != desc_ring.tail_id) {
        const ros::StringView data {log::retrieve_log<print_data_size, print_desc_size>(s_prb)};
        if (s_sink.valid)
        {
            s_sink.write(data);
        }
    }
}


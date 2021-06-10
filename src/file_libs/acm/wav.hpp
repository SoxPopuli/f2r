#pragma once
#include "config.hpp"
#include "common/str_array.hpp"

namespace file
{
    using namespace str_array;

    struct WAV
    {
        struct Header
        {
            typedef const std::array<char, 4> MarkerType;

            MarkerType four_cc = "RIFF"_str_array;
            u32 file_size;
            MarkerType file_type = "WAVE"_str_array;
            MarkerType fmt_chunk_marker = "fmt\0"_str_array;
            u32 fmt_data_len;
            u16 fmt_type;
            u16 channel_count;
            u32 sample_rate;
            u32 channel_bytes;      // (sample_rate * bits_per_sample * channel_count) / 8
            u16 bit_stride;         // (bits_per_sample * channel_count) / frame_bits [16/8 bit]
            u16 bits_per_sample;
            MarkerType data_chunk_header = "data"_str_array;
            u16 data_size;          // file_size - header_size
        } header;

    };

}

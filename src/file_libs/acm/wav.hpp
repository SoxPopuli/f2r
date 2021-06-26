/*
   specification found from
   http://soundfile.sapp.org/doc/WaveFormat/
*/
#pragma once
#include "config.hpp"
#include "common/str_array.hpp"

#include <vector>

namespace file
{
    using namespace str_array;
    struct ACM;

    struct WAV
    {
        struct Header
        {
            typedef const std::array<char, 4> MarkerType;

            MarkerType  chunk_id = "RIFF"_str_array;                                                 
            u32         chunk_size; //= 4 + (8 + format_chunk_size) + (8 + data_chunk_size)
            MarkerType  format = "WAVE"_str_array;                                                 
            MarkerType  format_chunk_id = "fmt "_str_array;                                                
            u32         format_chunk_size = 16;
            // format chunk start
            u16         audio_format;                                                                                
            u16         channel_count;                                                                           
            u32         sample_rate; //usually 22050 for acm                                                                           
            u32         byte_rate;   //= (sample_rate * bits_per_sample * channel_count) / 8
            u16         block_align; //= (bits_per_sample * channel_count) / frame_bits [16/8 bit]
            u16         bits_per_sample;                                                                         
            // format chunk end
            MarkerType  data_chunk_id = "data"_str_array;                                                 
            u16         data_chunk_size; //= sample_count * channel_count * bits_per_sample / 8
        } header;

        // data chunk start
        std::vector<u8> data;
        // data chunk end

        WAV() = default;
        ~WAV() = default;

        explicit WAV( const ACM& acm );
    };

}

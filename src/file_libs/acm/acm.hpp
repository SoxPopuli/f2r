#pragma once
#include <config.hpp>
#include "wav.hpp"
#include "common/common.hpp"

#include <array>
#include <istream>

namespace file
{

    struct ACM: public FileBase<cfg::endian::little>
    {
        std::array<u8, 4> signature;
        u32 sample_count;
        u16 channel_num;
        u16 frequency;

        u8 attrs1;
        u16 attrs2;


        ACM() = default;
        ~ACM() = default;

        explicit ACM( std::istream& stream );

        static ACM open(const std::string& path);
    };

    WAV decode_acm( std::istream& stream );
}

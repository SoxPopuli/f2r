#pragma once
#include <config.hpp>
#include "common/common.hpp"

#include <array>

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



        static ACM open(const std::string& path);
    };

}

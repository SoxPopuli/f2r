#pragma once
#include "files/common.hpp"
#include "files/color.hpp"

#include <string>
#include <array>
#include <unordered_set>

namespace file::pal
{

    /*
       Endian is irrelevant for palette files, as it contains
       no data longer than a byte
     */
    struct Pal: public FileBase<cfg::SYSTEM_ENDIAN>
    {
        //array of rgb colors for the palette
        std::array<u8Rgb, 256> colors;
        std::unordered_set<std::size_t> unused_indices;
        std::array<std::uint8_t, 32767> rgb_conversion_table;

        //returns the set of rgb values that represent transparency
        auto transparent_color() const { return colors[0]; }

        Pal() = default;
        ~Pal() = default;

        Pal(std::istream& input);

        static Pal open(std::istream& path);
    };
}

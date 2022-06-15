#pragma once
#include "config.hpp"
#include "files/point.hpp"
#include "files/color.hpp"

#include <vector>

namespace file
{

    struct Frame
    {
        u16 width;
        u16 height;
        u32 size;

        Point<u16> pixel_shift;
        std::vector<u8> color_indices;
    };

}

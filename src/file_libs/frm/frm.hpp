#pragma once
#include "config.hpp"
#include "frame.hpp"
#include "common/common.hpp"
#include "common/point.hpp"
#include "pal/pal.hpp"

#include <array>
#include <vector>
#include <optional>

namespace file
{
    struct FRM: public FileBase<cfg::endian::big>
    {
        std::optional<PAL> palette = std::nullopt;

        u32 version;
        u16 fps;
        u16 action_frame;
        u16 frames_per_direction;

        std::array<Point<u16>, 6> pixel_shift;
        std::array<u32, 6> frame_data_offset;

        std::vector<Frame> frames;

        static FRM open(const std::string& path);
    };

}

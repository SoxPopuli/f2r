#include "frm.hpp"
#include "frame.hpp"
#include "common/sequential_reader.hpp"

#include <cstdio>

namespace file
{

    FRM FRM::open(const std::string& path)
    {
        FRM f;
        auto bytes = get_file_bytes(path);
        auto r = SequentialReader(bytes, f);

        r.read_into(f.version);
        r.read_into(f.fps);
        r.read_into(f.action_frame);
        r.read_into(f.frames_per_direction);

        for(std::size_t i = 0; i < f.pixel_shift.size(); i++)
            f.pixel_shift[i].x = r.read<u16>();
        for(std::size_t i = 0; i < f.pixel_shift.size(); i++)
            f.pixel_shift[i].y = r.read<u16>();
        for(std::size_t i = 0; i < f.frame_data_offset.size(); i++)
            f.frame_data_offset[i] = r.read<u32>();

        [[maybe_unused]] auto size = r.read<u32>();

        while( r.get_offset() < bytes.size() )
        {
            Frame frame;
            r.read_into(frame.width);
            r.read_into(frame.height);
            r.read_into(frame.size);

            r.read_into(frame.pixel_shift.x);
            r.read_into(frame.pixel_shift.y);
            
            frame.color_indices.resize( frame.size );
            for(auto& index : frame.color_indices)
                r.read_into(index);

            f.frames.push_back(frame);
        }

        return f;
    }

}

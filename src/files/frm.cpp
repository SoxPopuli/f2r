#include "files/frm.hpp"
#include "files/frame.hpp"
#include "files/sequential_reader.hpp"
#include "files/common.hpp"

#include <cstdio>

namespace file::frm
{

    Frm::Frm(std::istream& stream)
    {
        auto bytes = get_file_bytes(stream);
        auto r = make_sequential_reader<Frm::endianness>(bytes);

        r.read_into(version);
        r.read_into(fps);
        r.read_into(action_frame);
        r.read_into(frames_per_direction);

        for(std::size_t i = 0; i < pixel_shift.size(); i++)
            pixel_shift[i].x = r.read<u16>();
        for(std::size_t i = 0; i < pixel_shift.size(); i++)
            pixel_shift[i].y = r.read<u16>();
        for(std::size_t i = 0; i < frame_data_offset.size(); i++)
            frame_data_offset[i] = r.read<u32>();

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

            frames.push_back(frame);
        }
    }

}

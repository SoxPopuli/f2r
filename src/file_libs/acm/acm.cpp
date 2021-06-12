#include "acm.hpp"
#include "common/sequential_reader.hpp"

#include <cassert>
#include <algorithm>

namespace file
{
    ACM::ACM( std::istream& stream )
    {
        auto bytes = get_file_bytes(stream);

        
    }

    ACM ACM::open(const std::string& path)
    {
        auto bytes = get_file_bytes(path);
        ACM acm;

        

        return acm;
    }

    WAV decode_acm( std::istream& stream )
    {
        struct ACM_EMPTY: public FileBase<cfg::endian::little> {};
        constexpr auto ACM_ENDIAN = ACM_EMPTY{}.endianness;
        const auto fix = [&](const auto& val) { return fix_endian<ACM_ENDIAN>(val); };
        auto bytes = get_file_bytes(stream);
        auto r = SequentialReader(bytes, ACM_EMPTY{});

        WAV wav;

        auto sig1 = std::to_array({
                bytes[0],
                bytes[1], 
                bytes[2]
        });
        r.set_offset(0x3);
        assert(
            sig1[0] == 0x97 and
            sig1[1] == 0x28 and
            sig1[2] == 0x03
        );
        auto sig2 = r.read<u8>();

        auto sample_count = r.read<u32>();

        //warning: value lies
        //channel count is always stereo (2) for music, 
        //and mono (1) for everything else
        [[maybe_unused]] u16 channel_count = r.read<u16>();
        auto frequency = r.read<u16>();
        auto attributes = r.read<u16>();

        auto encoding_level = 0b1111 & attributes;
        auto block_count = ((~0b1111) & attributes) >> 4;

        auto sub_block_size = 1 << encoding_level;
        auto packed_block_size = sub_block_size * block_count;
        auto buffer_size = 3 * sub_block_size/2 - 2;

        //u32 packed_block[block_count][sub_block_size];
        std::vector<u32> packed_block( static_cast<size_t>(block_count * sub_block_size) );
        auto pb_index = [&](auto x, auto y) -> u32&
        {
            const auto width = block_count;
            return packed_block[ width * y + x ];
        };

        std::vector<u32> decoding_buffer( static_cast<size_t>(buffer_size) );

        if(encoding_level != 0)
        {

        }

        return wav;
    }
}

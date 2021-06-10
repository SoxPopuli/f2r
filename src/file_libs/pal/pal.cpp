#include "pal.hpp"
#include <cstring>

namespace file
{

    PAL PAL::open(const std::string& path)
    {
        auto bytes = get_file_bytes(path);
        PAL pal;

        const auto get_color = [&bytes](auto index)
        {
            constexpr std::uint8_t color_mult = 4;
            auto r = bytes[ (index*3) + 0 ] * color_mult;
            auto g = bytes[ (index*3) + 1 ] * color_mult;
            auto b = bytes[ (index*3) + 2 ] * color_mult;
            return u8Rgb{
                .r = static_cast<std::uint8_t>(r),
                .g = static_cast<std::uint8_t>(g),
                .b = static_cast<std::uint8_t>(b),
            };
        };
        const auto check_color = [&pal](u8Rgb& rgb, auto index)
        {
            constexpr auto limit = (63 * 4);
            if(rgb.r > limit || rgb.g > limit || rgb.b > limit)
            {
                rgb = {0, 0, 0};
                pal.unused_indices.insert(index);
            }
        };
        
        for(std::size_t i = 0; i < 256; i++)
        {
            auto& col = pal.colors[i];
            col = get_color(i);
            
            check_color(col, i);
        }


        std::size_t index = 0;
        //for(auto* elem = bytes.data() + 0x300;
                  //elem <= bytes.data() + 0x82FF;
                  //elem++)
        for(auto e = 0x300; e < 0x82FF; e++)
        {
            pal.rgb_conversion_table[index++] = bytes[e];
        }

        return pal;
    }

}

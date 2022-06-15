#include "files/pal.hpp"
#include <cstring>
#include <istream>

namespace file::pal
{

    Pal::Pal(std::istream& input)
    {
        auto bytes = get_file_bytes(input);

        constexpr std::uint8_t color_mult = 4;
        constexpr auto limit = (63 * color_mult);
        const auto get_color = [&bytes](auto index)
        {
            auto r = bytes[ (index*3) + 0 ] * color_mult;
            auto g = bytes[ (index*3) + 1 ] * color_mult;
            auto b = bytes[ (index*3) + 2 ] * color_mult;
            return u8Rgb{
                .r = static_cast<std::uint8_t>(r),
                .g = static_cast<std::uint8_t>(g),
                .b = static_cast<std::uint8_t>(b),
            };
        };
        const auto check_color = [&](u8Rgb& rgb, auto index)
        {
            if(rgb.r > limit || rgb.g > limit || rgb.b > limit)
            {
                rgb = {0, 0, 0};
                unused_indices.insert(index);
            }
        };
        
        for(std::size_t i = 0; i < 256; i++)
        {
            auto& col = colors[i];
            col = get_color(i);
            
            check_color(col, i);
        }


        std::size_t index = 0;
        //for(auto* elem = bytes.data() + 0x300;
                  //elem <= bytes.data() + 0x82FF;
                  //elem++)
        for(auto e = 0x300; e < 0x82FF; e++)
        {
            rgb_conversion_table[index++] = bytes[e];
        }
    }

}

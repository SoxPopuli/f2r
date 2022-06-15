#include "files/frm.hpp"
#include "files/pal.hpp"

#include <cxxopts.hpp>
#include <iostream>
#include <cstdint>
#include <array>
#include <cstdio>
#include <iostream>
#include <fstream>

//#include <OpenImageIO/imageio.h>

#include "stb/stb_image_write.h"

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

using std::array;
struct RGBA { 
    u8 r, g, b, a;
};

//void write_image(const file::FRM& f)
//{
    //using namespace OIIO;
    //std::vector<RGBA> pixels;
    //auto f0 = f.frames[0];
    //for(const auto& ci : f0.color_indices)
    //{
        //auto p = f.palette->colors[ci];
        //u8 alpha = 255;
        //if( ci == 0 ) alpha = 0;
        //pixels.push_back( RGBA{ p.r, p.g, p.b, alpha } );
    //}

    //const auto width = f0.width;
    //const auto height = f0.height;
    //constexpr int channels = 4;
    //const auto filename = "frm.png";

    //auto output = ImageOutput::create(filename);
    //auto spec = ImageSpec(width, height, channels, TypeDesc::UINT8);
    //output->open(filename, spec);
    //output->write_image(TypeDesc::UINT8, pixels.data());
    //output->close();
//}

void write_image_stb(const file::frm::Frm& f)
{
    std::vector<RGBA> pixels;
    auto f0 = f.frames[0];
    for(const auto& ci : f0.color_indices)
    {
        auto p = f.palette->colors[ci];
        u8 alpha = 255;
        if( ci == 0 ) alpha = 0;
        pixels.push_back( RGBA{ p.r, p.g, p.b, alpha } );
    }

    const auto width = f0.width;
    const auto height = f0.height;
    constexpr int channels = 4;

    auto write_func = [](void*, void* data, int size)
    {
        auto so = std::freopen(nullptr, "wb", stdout);
        std::fwrite(data, 1, size, so);
    };
    stbi_write_png_to_func(write_func, nullptr, f0.width, f0.height, 4, pixels.data(), 0);
    //stbi_write_png("frm.png", width, height, channels, pixels.data(), 0);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::printf("missing args\n");
        std::printf("e.g. frm_to_img pal frm\n");
        return 1;
    }

    auto pal_stream = std::ifstream(argv[1]);
    auto frm_stream = std::ifstream(argv[2]);

    auto pal = file::pal::Pal( pal_stream );
    auto frm = file::frm::Frm( frm_stream );
    frm.palette = pal;

    //write_image(frm);
    write_image_stb(frm);
    return 0;
}

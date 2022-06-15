#pragma once
#include <cstdint>

namespace file
{
    template<class T>
    struct Rgb { T r, g, b; };

    typedef Rgb<int> iRgb;
    typedef Rgb<float> fRgb;
    typedef Rgb<std::uint8_t> u8Rgb;
    typedef Rgb<std::uint16_t> u16Rgb;
    typedef Rgb<std::uint32_t> u32Rgb;
    typedef Rgb<std::int8_t> s8Rgb;
    typedef Rgb<std::int16_t> s16Rgb;
    typedef Rgb<std::int32_t> s32Rgb;

    template<class T>
    struct Rgba { T r, g, b, a; };

    typedef Rgba<int> iRgba;
    typedef Rgba<float> fRgba;
    typedef Rgba<std::uint8_t> u8Rgba;
    typedef Rgba<std::uint16_t> u16Rgba;
    typedef Rgba<std::uint32_t> u32Rgba;
    typedef Rgb<std::int8_t> s8Rgb;
    typedef Rgb<std::int16_t> s16Rgb;
    typedef Rgb<std::int32_t> s32Rgb;
}


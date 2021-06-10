#pragma once
#include "config.hpp"

#include <algorithm>
#include <array>
#include <type_traits>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>

namespace file
{
    template<cfg::endian Endian, std::integral T>
    auto fix_endian(const T& value) -> 
        std::enable_if_t<Endian != cfg::SYSTEM_ENDIAN, T>
    {
        constexpr auto byte_len = sizeof(T);

        std::array<std::uint8_t, byte_len> bytes;
        std::memcpy(bytes.data(), &value, byte_len);
        std::reverse(bytes.begin(), bytes.end());

        T outval;
        std::memcpy(&outval, bytes.data(), byte_len);
        return outval;
    }

    template<cfg::endian Endian, std::integral T>
    constexpr auto fix_endian(const T& value) -> 
        std::enable_if_t<Endian == cfg::SYSTEM_ENDIAN, T>
    {
        return value;
    }

    template<cfg::endian Endian>
    struct FileBase
    {
        constexpr static cfg::endian endianness = Endian;

        template<std::integral T>
        auto fix_endian(const T& value) const
        {
            return ::file::fix_endian<Endian>(value);
        }

    };

    std::vector<std::uint8_t> get_file_bytes(const std::string& path);
}

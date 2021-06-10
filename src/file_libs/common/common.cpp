#include "common.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <string>

namespace file
{
    std::vector<std::uint8_t> get_file_bytes(const std::string& path)
    {
        using namespace std::string_literals;
        if( !std::filesystem::exists(path) )
        {
            auto msg = "File not found: "s + path;
            throw std::runtime_error(msg);
        }

        constexpr auto open_mode = std::ios::in | 
            std::ios::binary |
            std::ios::ate;
        auto input_stream = std::ifstream(path, open_mode);

        auto end_pos = input_stream.tellg();
        input_stream.seekg(0);

        std::vector<std::uint8_t> data;
        data.resize(end_pos);

        input_stream.read(
                reinterpret_cast<char*>(data.data()),
                end_pos
            );

        return data;
    }

}


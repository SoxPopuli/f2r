#include "files/common.hpp"
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

        constexpr auto open_mode = 
            std::ios::in | 
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

    std::vector<std::uint8_t> get_file_bytes(std::istream& stream)
    {
        if( !stream )
            throw std::runtime_error("Failed to read stream");

        stream.seekg( 0, std::ios::end );
        auto end_pos = stream.tellg();
        stream.seekg(0);
        
        std::vector<std::uint8_t> data( static_cast<std::size_t>(end_pos) );
        stream.read( 
                reinterpret_cast<char*>(data.data()),
                end_pos
        );

        return data;
    }
}


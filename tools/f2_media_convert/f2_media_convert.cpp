#include "acm/acm.hpp"
#include "acm/wav.hpp"

#include <libacm.hpp>

#include <cstdlib>
#include <cxxopts.hpp>
#include <fmt/core.h>
#include <exception>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>


class ArgumentException: public std::exception 
{
    std::string _msg;
public:
    ArgumentException(const std::string& msg): 
        _msg(msg) { }

    const char* what() const noexcept override 
    {
        return _msg.c_str();
    }
};
class HelpException: public std::exception
{
    std::string _msg;
public:
    HelpException(const std::string& msg): _msg(msg) {}
    const char * what() const noexcept override
    {
        return _msg.c_str();
    }
};

auto parse_args(int argc, char* argv[])
{
    cxxopts::Options options("f2_media_convert", 
            "convert acm/mve to mp3/mp4");

    const auto string_arg = [](){ return cxxopts::value<std::string>(); };

    options.add_options()
        ("i,input", "input file", string_arg())
        ("o,output", "output file, uses stdout by default", string_arg())
        ("c,channels", "override channels", cxxopts::value<int>())
        ("t,test", "print file info")
        ("h,help", "show help")
        ;

    options.custom_help("-i [input file] [OPTIONS]");
    auto result = options.parse(argc, argv);

    if( result.count("help") ) 
    {
        throw HelpException( options.help() );
    }

    using namespace std::string_literals;
    std::string arg_err_str = "";
    if( result.count("i") == 0 ) 
        arg_err_str += "input ";
    if( arg_err_str.size() > 0 ) { throw ArgumentException("missing argument(s): "s + arg_err_str); }

    return result;
}

static void write_wav_header(acm::Acm& acm, std::ostream& out)
{
    constexpr auto code = 1;
    auto chars = [](const auto& data) { return reinterpret_cast<const char*>(&data); };
    auto write32 = [&](const auto& data) { out.write(chars(data), 4); };
    auto write16 = [&](const auto& data) { out.write(chars(data), 2); };

    auto channels = acm.channels();
    auto data_len = acm.size();
    auto rate = acm.rate();
    auto avg_bps = acm.bits_per_second();
    auto bits = acm.word_size() * 8;
    auto block_align = bits * channels / 8;
    auto header_len = 16;
    auto wav_len = 4 + 8 + header_len + 8 + data_len;

    out.write("RIFF", 4);
    write32(wav_len);
    out.write("WAVEfmt ", 8);
    write32(header_len);
    write16(code);
    write16(channels);
    write32(rate);
    write32(avg_bps);
    write16(block_align);
    write16(bits);

    out.write("data", 4);
    write32(data_len);
}

int main(int argc, char* argv[])
{
    try {
        auto result = parse_args(argc, argv);

        auto input = result["input"].as<std::string>();
        auto stream = std::ifstream(input, std::ios::in | std::ios::binary);

        int channels = -1;
        if( result.count("channels") > 0 )
            channels = result["channels"].as<int>();

        auto acm = acm::Acm(stream, channels);
        acm.write_wav(std::cout);

    } catch( HelpException& ex ) {
        fmt::print("{}\n", ex.what());
        return EXIT_SUCCESS;
    } catch( std::exception& ex ) {
        fmt::print("{}\n", ex.what());
        return EXIT_FAILURE;
    } 
    return EXIT_SUCCESS;
}

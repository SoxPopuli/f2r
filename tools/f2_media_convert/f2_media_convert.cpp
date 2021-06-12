#include "acm/acm.hpp"
#include "acm/wav.hpp"

#include <cstdlib>
#include <cxxopts.hpp>
#include <fmt/core.h>
#include <exception>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>


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

int main(int argc, char* argv[])
{
    try {
        auto result = parse_args(argc, argv);

        if( result.count("test") > 0 )
        {
            auto input = result["input"].as<std::string>();
            auto stream = std::ifstream(input, std::ios::in | std::ios::binary);
            auto data = file::decode_acm(stream);

            fmt::print("{}:\n", input);


        }

    } catch( HelpException& ex ) {
        fmt::print("{}\n", ex.what());
        return EXIT_SUCCESS;
    } catch( std::exception& ex ) {
        fmt::print("{}\n", ex.what());
        return EXIT_FAILURE;
    } 
    return EXIT_SUCCESS;
}

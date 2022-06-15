#include "files/frm.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <cxxopts.hpp>
#include <fstream>

int main(int argc, char* argv[])
{
    cxxopts::Options options("read_frm", "read f2 frm file");
    options.add_options()
        ("h,help", "show help")
        ("file", "file to open", cxxopts::value<std::string>());

    options.parse_positional("file");
    auto result = options.parse(argc, argv);

    if( result.count("help") )
    {
        fmt::print("{}\n", options.help());
        return 2;
    }

    auto frm_stream = std::ifstream( result["file"].as<std::string>() );
    auto frm = file::frm::Frm( frm_stream );
    fmt::print("version:        {}\n", frm.version);
    fmt::print("fps:            {}\n", frm.fps);
    fmt::print("action_frame:   {}\n", frm.action_frame);
    fmt::print("frames per dir: {}\n", frm.frames_per_direction);

    fmt::print("pixel_shifts: {{\n");
    for(const auto p : frm.pixel_shift)
        fmt::print("    x: {}, y: {}\n", p.x, p.y);
    fmt::print("}}\n");

    fmt::print("frame_data_offsets: {{\n");
    for(const auto o : frm.frame_data_offset)
        fmt::print("    {}\n", o);
    fmt::print("}}\n");

    return 0;
}

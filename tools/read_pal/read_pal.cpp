#include "pal/pal.hpp"
#include <cxxopts.hpp>
#include <fmt/core.h>
#include <fstream>

file::pal::Pal palette;

void print_text()
{
    int index = 0;
    for(const auto& c : palette.colors)
    {
        fmt::print("Color[{:>3}]: R: {:>3} G: {:>3} B: {:>3}\n",
                index++,
                c.r, c.g, c.b);
    }

    fmt::print("{:─<40}\n", "");
    index = 0;
    for(const auto& u : palette.unused_indices)
    {
        fmt::print("Unused[{:>3}]: {:>3}\n",
                index++, u);
    }
}

void print_html()
{
    fmt::print("<body style=\"background-color: black;\">\n");

    int index = 0;
    for(const auto& c : palette.colors)
    {
        std::string font_color = "black";
        if( (c.r + c.g + c.b) < 60 )
            font_color = "white";
        if( palette.unused_indices.contains(index) == false )
            fmt::print("<p style=\""
                    "background-color: rgb({0}, {1}, {2});"
                    "color: {4};"
                    "\">{3}: {0}, {1}, {2}</p>\n",
                    c.r, c.g, c.b, index, font_color);

        index++;
    }

    fmt::print("</body>\n");
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fmt::print("Missing file arg\n");
        return 1;
    }

    cxxopts::Options options("read_pal", "read f2 palette files");
    options.add_options()
        ("w", "html output", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "show help")
        ("file", "file to open", cxxopts::value<std::string>());

    options.parse_positional("file");
    auto result = options.parse(argc, argv);

    if( result.count("help") )
    {
        fmt::print("{}\n", options.help());
        return EXIT_SUCCESS;
    }

    auto pal_stream = std::ifstream(result["file"].as<std::string>());
    palette = file::pal::Pal(pal_stream);

    if( result.count("w") == 0 )
        print_text();
    else
        print_html();

    return EXIT_SUCCESS;
}

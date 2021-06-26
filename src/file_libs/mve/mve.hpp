#pragma once
#include <integers.hpp>
#include <vector>

namespace file::mve
{
    struct Chunk;

    struct Mve
    {
        struct Header
        {

        } header;

        std::vector<Chunk> chunks;
    };
}

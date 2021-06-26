#pragma once
#include "integers.hpp"

#include <vector>

namespace file::mve
{
    struct Opcode;

    struct Chunk
    {
        u16 length;
        u16 type;

        std::vector<Opcode> opcodes;
    };
}

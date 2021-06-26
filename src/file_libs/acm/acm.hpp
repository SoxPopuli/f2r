#pragma once
#include <config.hpp>
#include "wav.hpp"
#include "common/common.hpp"

#include <array>
#include <istream>

namespace file
{
    WAV decode_acm( std::istream& stream );
}

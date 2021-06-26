#include <cstdio>
#include <cstdint>
#include <iostream>

#include "common/common.hpp"

#define assert(test) \
    if(! ( test ) ) \
    { \
        std::fprintf(stderr, "assertion failed: %s\n", #test ); \
        return 1; \
    } 


constexpr auto opposite_endian = 
cfg::SYSTEM_ENDIAN == cfg::endian::little ?
cfg::endian::big : cfg::endian::little;

struct Test : public file::FileBase<opposite_endian>
{
    int16_t value;
};

int main(int, char**)
{
    Test t;
    t.value = 0xFF;

    auto reversed = t.fix_endian(t.value);

    std::cout << "normal:   " << t.value << "\n";
    std::cout << "reversed: " << reversed << "\n";
    assert(reversed == -256);

    return 0;
}

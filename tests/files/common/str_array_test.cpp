#include "files/str_array.hpp"
#include <cassert>

int main(
    [[maybe_unused]] int argc, 
    [[maybe_unused]] char* argv[])
{
    using namespace file::str_array;

    constexpr auto test = "test"_str_array;
    constexpr auto expected = 
        std::to_array({'t', 'e', 's', 't'});

    assert( test.size() == expected.size() );
    assert( test == expected );
    

    return 0;
}

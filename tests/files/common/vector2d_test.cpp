#include "files/vector2d.hpp"

#include <cassert>
#include <cstring>

int main()
{
    using file::Vector2D;


    Vector2D<int> vec2d(9, 5);
    int array[9][5];

    struct pos { int x, y; };

    auto set_both = [&](int value, pos p) 
    {
        vec2d(p.x, p.y) = value;
        array[p.x][p.y] = value;
    };

    auto test_both = [&](pos p)
    {
        return vec2d(p.x, p.y) == array[p.x][p.y];
    };

    for(int col = 0; col < 9; col++)
    {
        int* subarray = array[col];

        for(int row = 0; row < 5; row++)
            subarray[row] = 0;
    }

    pos p = { 2, 2 };
    set_both(1, p);
    assert(test_both(p));

    p = {3, 4};
    set_both(2, p);
    assert(test_both(p));

    p = {4, 3};
    set_both(3, p);
    assert(test_both(p));

    return 0;
}

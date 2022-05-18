#pragma once
#include <vector>
#include <cstdio>

namespace file
{
    //std::vector but 2-dimensional
    //intended to be identical to array-of-arrays
    //for the purpose of indexing
    //
    //e.g. Vector2D[x, y] == CArray[x][y]
    template<class T>
    struct Vector2D: public std::vector<T>
    {
        typedef std::vector<T> super;
        size_t width, height;
        
        Vector2D(size_t width, size_t height):
            super(width * height),
            width(width),
            height(height)
        { }

        void set_bounds(size_t new_width, size_t new_height)
        {
            bool resize_vector = false;
            if( new_width * new_height != width * height )
                resize_vector = true;

            width = new_width;
            height = new_height;
            if(resize_vector)
                super::resize(width * height);
        }

        using super::operator[];
        T& operator()(size_t y, size_t x)
        {
            auto index =  (width * y) + x ;
            std::printf("index: %lu\n", index);
            return super::operator[](index);
        }
    };

}

#pragma once

#include <array>
#include <algorithm>
#include <cstdint>

namespace file
{

    namespace str_array
    {
        //Struct to hold char data
        //discards the null terminator
        template<std::size_t N>
        struct CharArray 
        {
            constexpr auto size() const { return N-1; }
            char ch[N-1];

            constexpr CharArray( const char(&cstr)[N] )
            {
                for( std::size_t i = 0; i < N-1; i++ )
                    ch[i] = cstr[i];
            }
        };

        template<CharArray ch>
        constexpr auto operator ""_str_array() 
        {
            std::array<char, ch.size()> arr;
            std::copy( std::begin(ch.ch), std::end(ch.ch), arr.begin() );
            return arr;
        }

    }



}

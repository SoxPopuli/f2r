#pragma once
#include "files/common.hpp"

#include <cstdint>
#include <cstring>

namespace file
{

    //template<class Indexable, cfg::endian Endian>
    template<cfg::endian Endian, class Indexable>
    class SequentialReader
    {
        std::size_t offset = 0;
    public:
        const Indexable& indexable;
        //const FileBase<Endian>& file;

        SequentialReader(const Indexable& indexable) :
            indexable(indexable) {}

        ~SequentialReader() = default;

        template<class T>
        T read()
        {
            T value;
            std::memcpy(&value, &(indexable[offset]), sizeof(T));

            offset += sizeof(T);
            return fix_endian<Endian>(value);
        }

        template<class T>
        void read_into(T& value)
        {
            std::memcpy(&value, &(indexable[offset]), sizeof(T));

            value = fix_endian<Endian>(value);
            offset += sizeof(T);
        }

        std::size_t get_offset() const { return offset; }
        void set_offset(std::size_t new_offset) { offset = new_offset; }
    };

    template<cfg::endian Endian, class Indexable>
    auto make_sequential_reader(const Indexable& indexable)
    {
        return SequentialReader<Endian, Indexable>(indexable);
    }
}

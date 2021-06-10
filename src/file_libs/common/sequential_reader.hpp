#pragma once
#include "common.hpp"

#include <cstdint>
#include <cstring>

namespace file
{

    template<class Indexable, cfg::endian Endian>
    class SequentialReader
    {
        std::size_t offset = 0;
    public:
        const Indexable& indexable;
        const FileBase<Endian>& file;

        SequentialReader(const Indexable& indexable, const FileBase<Endian>& endian): 
            indexable(indexable), file(endian)
        {}

        ~SequentialReader() = default;

        template<class T>
        T read()
        {
            T value;
            std::memcpy(&value, &(indexable[offset]), sizeof(T));

            offset += sizeof(T);
            return file.fix_endian(value);
        }

        template<class T>
        void read_into(T& value)
        {
            std::memcpy(&value, &(indexable[offset]), sizeof(T));

            value = file.fix_endian(value);
            offset += sizeof(T);
        }

        std::size_t get_offset() const { return offset; }
        void set_offset(std::size_t new_offset) { offset = new_offset; }
    };

}

#include "acm.hpp"
#include "common/sequential_reader.hpp"
#include "common/vector2d.hpp"

#include <cassert>
#include <cstring>
#include <algorithm>
#include <memory>
#include <array>
#include <ranges>
#include <stack>
#include <optional>

struct Offset 
{
    size_t bits;

    size_t as_bytes() const { return bits / 8; }
    u8 remainder() const { return bits % 8; }

    Offset& operator+=(const size_t& s) {
        bits += s;
        return *this;
    }

    u64 get_data(const u8* start, int bits_requested) const
    {
        u64 mask = 0;
        auto bit_offset = remainder();

        //mask off lower end then invert mask
        for(int i = 0; i < bits_requested; i++)
        {
            mask |= 1 << i;
        }
        mask <<= bit_offset;

        size_t bytes_to_copy;
        //get bytes to copy into data
        //if bit data is not divisible by 8, round up
        if(bits_requested % 8 == 0)
            bytes_to_copy = bits_requested / 8;
        else 
            bytes_to_copy = ( bits_requested / 8 ) + 1;

        u64 data = 0;

        //copy unmasked bytes into data
        //then mask and shift
        std::memcpy(&data, start + as_bytes(), bytes_to_copy);
        //data = *reinterpret_cast<u64*>(start + as_bytes());
        data &= mask;
        data >>= bit_offset;

        return data;
    }

    auto read_data(const u8* start, int bits_requested)
    {
        auto d = get_data(start, bits_requested);
        this->bits += bits_requested;
        return d;
    }
};


namespace file
{
    typedef u32 PackedBlockElemType;
    typedef std::unique_ptr<PackedBlockElemType[]> PackedBlockSubarrayType;
    typedef std::unique_ptr<PackedBlockSubarrayType[]> PackedBlockType;

    struct Acm : public FileBase<cfg::endian::little>
    {
        u32 sample_count;
        u16 channel_count;
        u16 frequency;
        u16 attributes;
        u8  encoding_level;
        u16 block_count;

        size_t sub_block_size;
        size_t packed_block_size;
        size_t decoding_buffer_size;

        PackedBlockType packed_block;
        std::vector<u32> decoding_buffer;
        std::vector<u16> amp_buffer = std::vector<u16>( static_cast<size_t>(0x10'000) );

        auto& buffer_middle(size_t index) { return amp_buffer[0x8'000 + index]; }

        const auto& columns() const { return sub_block_size; }
        const auto& rows() const { return block_count; }
    };

    typedef void(*FillFunc)(const struct FillerArgs&);
    struct FillerArgs
    {
        const std::vector<u8>& bytes;
        Offset& offset;
        Acm& acm;
        u64 filler_index;
        size_t column;

        auto read_bits(int bits) const
        {
            return offset.read_data(bytes.data(), bits);
        }
    };

    auto convert_base(int number, int new_base)
    {
        std::stack<int> digits;
        int rem;
        int div;
        do
        {
            rem = number % new_base;
            div = number / new_base;
            digits.push(rem);
        }while( div > 0 );

        return digits;
    }

    void fill_zero(const FillerArgs& args) 
    {
        for(int i = 0; i < args.acm.rows(); i++)
            args.acm.packed_block[i][args.column] = 0;
    }
    void fill_ret0([[maybe_unused]] const FillerArgs& args) 
    {
        return;
    }
    void fill_linear(const FillerArgs& args) 
    {
        for(int i = 0; i < args.acm.rows(); i++)
        {
            auto val = args.offset.read_data(args.bytes.data(), args.filler_index);
            args.acm.packed_block[i][args.column] = args.acm.buffer_middle(val);
        }
    }

    struct BitSeq 
    {
        std::vector<bool> bits;
        int value;
        std::optional<int> value2 = std::nullopt;
    };
    struct KTrieData
    {
        int value;
        std::optional<int> value2 = std::nullopt;
    };
    struct KTrieNode
    {
        bool key;
        std::unique_ptr<KTrieNode> children[2] = { nullptr, nullptr };
        std::optional<KTrieData> data = std::nullopt;
    };
    struct KTrie
    {
        KTrieNode root;

        KTrie() = default;

    private:
        void add_seq(const BitSeq& seq)
        {
            auto* node = &root;
            for(size_t i = 0; i < seq.bits.size(); i++)
            {
                const auto& bit = seq.bits[i];
                if( not node->children[bit] )
                {
                    auto bit_ptr = std::make_unique<KTrieNode>();
                    bit_ptr->key = bit;
                    node->children[bit] = std::move(bit_ptr);
                }
                node = node->children[bit].get();
            }
            //set data to last node
            node->data = KTrieData{ .value = seq.value, .value2 = seq.value2 };
        }

    public:

        KTrie(const std::vector<BitSeq>& sequences)
        {
            //traverse bit_seq, adding non-existant nodes
            for(const auto& seq : sequences)
            {
                add_seq(seq);
            }
        }
    };

    void k_fill(const FillerArgs& args, const KTrie& trie)
    {
        auto read_bit = [&]() { return static_cast<bool>(args.read_bits(1)); };
        auto get_data = [&]()
        {
            const KTrieNode* current_node = &trie.root;
            while(true)
            {
                auto bit = read_bit();
                if( auto& c = current_node->children[bit] )
                {
                    current_node = c.get();
                    if( current_node->data )
                        return current_node->data.value();
                    continue;
                }
                else throw std::runtime_error("bit sequence not found");
            }
        };

        for(int i = 0; i < args.acm.rows(); i++)
        {
            auto data = get_data();
            args.acm.packed_block[i][args.column] = data.value;
            if(data.value2)
                args.acm.packed_block[++i][args.column] = data.value2.value();
        }
    }

    void fill_k12(const FillerArgs& args) 
    {
        auto trie = KTrie{ std::vector<BitSeq>{
            { {0}, 0 },
            { {1, 0}, args.acm.buffer_middle(-1) },
            { {1, 1}, args.acm.buffer_middle(+1) }
        }};
        k_fill(args, trie);
    }
    void fill_k13(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { .bits = {0}, .value = 0, .value2 = 0 },
            { .bits = {1, 0}, .value = 0 },
            { {1, 1, 0}, args.acm.buffer_middle(-1) },
            { {1, 1, 1}, args.acm.buffer_middle(+1) }
        }));
    }
    void fill_k23(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { {0}, 0 },
            { {1, 0, 0}, args.acm.buffer_middle(-2) },
            { {1, 1, 0}, args.acm.buffer_middle(-1) },
            { {1, 0, 1}, args.acm.buffer_middle(+1) },
            { {1, 1, 1}, args.acm.buffer_middle(+2) }
        }));
    }
    void fill_k24(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { {0}, 0, 0 },
            { {1, 0}, 0 },
            { {1, 1, 0, 0}, args.acm.buffer_middle(-2) },
            { {1, 1, 1, 0}, args.acm.buffer_middle(-1) },
            { {1, 1, 0, 1}, args.acm.buffer_middle(+1) },
            { {1, 1, 1, 1}, args.acm.buffer_middle(+2) }
        }));
    }
    void fill_k34(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { {0}, 0 },
            { {1, 0, 0}, args.acm.buffer_middle(-1) },
            { {1, 0, 1}, args.acm.buffer_middle(-1) },

            { {1, 1, 0, 0}, args.acm.buffer_middle(-3) },
            { {1, 1, 1, 0}, args.acm.buffer_middle(-2) },
            { {1, 1, 0, 1}, args.acm.buffer_middle(+2) },
            { {1, 1, 1, 1}, args.acm.buffer_middle(+3) },
        }));
    }
    void fill_k35(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { {0}, 0, 0 },
            { {1, 0}, 0 },
            { {1, 1, 0, 0}, args.acm.buffer_middle(-1) },
            { {1, 1, 0, 1}, args.acm.buffer_middle(+1) },

            { {1, 1, 1, 0, 0}, args.acm.buffer_middle(-3) },
            { {1, 1, 1, 1, 0}, args.acm.buffer_middle(-2) },
            { {1, 1, 1, 0, 1}, args.acm.buffer_middle(+2) },
            { {1, 1, 1, 1, 1}, args.acm.buffer_middle(+3) },
        }));
    }
    void fill_k44(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { {0}, 0 },

            { {1, 0, 0, 0}, args.acm.buffer_middle(-4) },
            { {1, 1, 0, 0}, args.acm.buffer_middle(-3) },
            { {1, 0, 1, 0}, args.acm.buffer_middle(-2) },
            { {1, 1, 1, 0}, args.acm.buffer_middle(-1) },

            { {1, 0, 0, 1}, args.acm.buffer_middle(+1) },
            { {1, 1, 0, 1}, args.acm.buffer_middle(+2) },
            { {1, 0, 1, 1}, args.acm.buffer_middle(+3) },
            { {1, 1, 1, 1}, args.acm.buffer_middle(+4) },
        }));
    }
    void fill_k45(const FillerArgs& args) 
    {
        k_fill(args, KTrie({
            { {0}, 0, 0 },
            { {1, 0}, 0 },

            { {1, 1, 0, 0, 0}, args.acm.buffer_middle(-4) },
            { {1, 1, 1, 0, 0}, args.acm.buffer_middle(-3) },
            { {1, 1, 0, 1, 0}, args.acm.buffer_middle(-2) },
            { {1, 1, 1, 1, 0}, args.acm.buffer_middle(-1) },

            { {1, 1, 0, 0, 1}, args.acm.buffer_middle(+1) },
            { {1, 1, 1, 0, 1}, args.acm.buffer_middle(+2) },
            { {1, 1, 0, 1, 1}, args.acm.buffer_middle(+3) },
            { {1, 1, 1, 1, 1}, args.acm.buffer_middle(+4) },
        }));
    }

    void t_fill(const FillerArgs& args, int bits, int base)
    {
        for(int i = 0; i < args.acm.rows(); i++)
        {
            auto data = args.read_bits(bits);
            auto digits = convert_base(data, base);
            int row = 0;
            while(digits.size() > 0)
            {
                args.acm.packed_block[row][args.column] = args.acm.buffer_middle( digits.top() );
                digits.pop();
                row += 1;
            }
        }
    }

    void fill_t15(const FillerArgs& args) { t_fill(args, 5, 3); }
    void fill_t27(const FillerArgs& args) { t_fill(args, 7, 5); }
    void fill_t37(const FillerArgs& args) { t_fill(args, 7, 11); }

    FillFunc get_fill_func(int index)
    {
        constexpr auto fillers = std::to_array<FillFunc>({
            fill_zero,    fill_ret0,    fill_ret0,    fill_linear,
            fill_linear,  fill_linear,  fill_linear,  fill_linear,
            fill_linear,  fill_linear,  fill_linear,  fill_linear,
            fill_linear,  fill_linear,  fill_linear,  fill_linear,
            fill_linear,  fill_k13,     fill_k12,     fill_t15,
            fill_k24,     fill_k23,     fill_t27,     fill_k35,
            fill_k34,     fill_ret0,    fill_k45,     fill_k44,
            fill_ret0,    fill_t37,     fill_ret0,    fill_ret0
        });

        return fillers[index];
    }


    void unpack(u32* buffer, u32* block, size_t columns, size_t rows)
    {

    }

    void unpack_values(Acm& acm)
    {
        u32 step, sub_count, sub_len;

        if( acm.encoding_level > 9 )
            step = 1;
        else
            step = (2048 >> acm.encoding_level) - 2;

        u32 todo = acm.rows();
        auto* block_ptr = acm.packed_block.get();
        u32* buffer_ptr = nullptr;

        while( true )
        {
            buffer_ptr = acm.decoding_buffer.data();
            sub_count = step;
            if( sub_count > todo )
                sub_count = todo;

            sub_len = acm.columns() / 2;
            sub_count *= 2;

            //unpack(buffer_ptr, block_ptr, sub_len, sub_count);
            //buffer_ptr += sub_len*2;
        }
    }

    WAV decode_acm( std::istream& stream )
    {
        Acm acm;
        auto bytes = get_file_bytes(stream);
        auto r = make_sequential_reader<cfg::endian::little>(bytes);

        WAV wav;

        auto sig1 = std::to_array({
                bytes[0],
                bytes[1], 
                bytes[2]
        });
        r.set_offset(0x3);
        assert(
            sig1[0] == 0x97 and
            sig1[1] == 0x28 and
            sig1[2] == 0x03
        );
        [[maybe_unused]] auto sig2 = r.read<u8>();

        acm.sample_count = r.read<u32>();

        //warning: value lies
        //channel count is always stereo (2) for music, 
        //and mono (1) for everything else
        acm.channel_count = r.read<u16>();
        acm.frequency = r.read<u16>();
        auto attributes = r.read<u16>();

        //packAttrs
        acm.encoding_level = 0b1111 & attributes;
        //packAttrs2
        acm.block_count = ((~0b1111) & attributes) >> 4;

        //someSize
        acm.sub_block_size = 1 << acm.encoding_level;
        //someSize2
        acm.packed_block_size = acm.sub_block_size * acm.block_count;
        //decBuff_size
        acm.decoding_buffer_size = 3 * acm.sub_block_size/2 - 2;

        acm.packed_block = std::make_unique< PackedBlockSubarrayType[] >( acm.columns() );
        for(size_t i = 0; i < acm.columns(); i++)
        {
            acm.packed_block[i] = std::make_unique<PackedBlockElemType[]>(acm.rows());
        }

        acm.decoding_buffer = std::vector<u32>();

        if(acm.encoding_level != 0)
        {
            acm.decoding_buffer.resize( acm.decoding_buffer_size );
        }

        auto offset = Offset{ .bits = r.get_offset() * 8 };

        auto read_bits = [&](auto bits) { return offset.read_data( bytes.data(), bits ); };
        //read until end
        while(offset.as_bytes() < bytes.size())
        {
            auto pwr = offset.read_data( bytes.data(), 4 );
            auto val = offset.read_data( bytes.data(), 16 );

            auto count = 1 << pwr;
            
            for(auto i = -count; i < count; i++)
            {
                acm.buffer_middle(i) = i * val;
            }


            for(size_t i = 0; i < acm.columns(); i++)
            {
                auto index = read_bits(5);
                auto filler = get_fill_func(index);

                if( filler == fill_ret0 )
                    throw std::runtime_error("error");

                filler(FillerArgs{
                    .bytes = bytes,
                    .offset = offset,
                    .acm = acm,
                    .filler_index = index,
                    .column = i
                });
            }
        }

        if( acm.encoding_level == 0 )
        {

        }
        else 
        {
            unpack_values(acm);
        }

        return wav;
    }
}

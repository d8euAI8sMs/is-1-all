#pragma once

#include <bitset>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

namespace common
{

    using block_t = std::bitset<64>;
    using blocks_t = std::vector<block_t>;

    using string_t = std::string;

    enum class byte_view_t { hex = 0, dec = 1, bin = 2 };



    inline blocks_t split(const string_t &str)
    {
        size_t str_size = str.length();
        bool tail = (str_size % 8) != 0;
        size_t parts = str_size / 8;

        blocks_t blocks(parts + (tail ? 1 : 0));
        for (size_t i = 0; i < parts; i++) // blocks
        {
            for (size_t j = 0; j < 8; j++) // bytes
            {
                for (size_t k = 0; k < 8; k++) // bits
                {
                    blocks[i][j * 8 + k] = ((str[i * 8 + j] >> (8 - k - 1)) & 0x1) == 0x1;
                }
            }
        }
        if (tail)
        {
            size_t tail_size = str_size % 8;
            for (size_t j = 0; j < tail_size; j++) // bytes
            {
                for (size_t k = 0; k < 8; k++) // bits
                {
                    blocks[parts][j * 8 + k] = ((str[parts * 8 + j] >> (8 - k - 1)) & 0x1) == 0x1;
                }
            }
        }
        return blocks;
    }




    inline string_t combine(const blocks_t &blocks)
    {
        std::ostringstream str;
        for (size_t i = 0; i < blocks.size(); i++) // blocks
        {
            for (size_t j = 0; j < 8; j++) // bytes
            {
                char c = 0;
                for (size_t k = 0; k < 8; k++) // bits
                {
                    c |= ((blocks[i][j * 8 + k] ? 1 : 0) << (8 - k - 1));
                }
                if (c != 0) str << c;
            }
        }
        return str.str();
    }




    inline block_t random_block()
    {
        block_t block;
        for (size_t i = 0; i < block.size(); i++)
        {
            block[i] = (std::rand() > (RAND_MAX / 2));
        }
        return block;
    }



    inline string_t to_string(const blocks_t blocks, byte_view_t view)
    {
        std::ostringstream str;

        for (size_t i = 0; i < blocks.size(); i++) // blocks
        {
            for (size_t j = 0; j < 8; j++) // chars
            {
                switch (view)
                {
                case common::byte_view_t::bin:
                    for (size_t k = 0; k < 8; k++) // bits
                    {
                        str << ((blocks[i][j * 8 + k] ? 1 : 0));
                    }
                    break;
                default:
                    unsigned int c = 0;
                    for (size_t k = 0; k < 8; k++) // bits
                    {
                        c |= ((blocks[i][j * 8 + k] ? 1 : 0) << (8 - k - 1));
                    }
                    switch (view)
                    {
                    case common::byte_view_t::hex:
                        str.width(2);
                        str.fill('0');
                        str << std::fixed << std::hex << c;
                        break;
                    case common::byte_view_t::dec:
                        str.width(3);
                        str.fill('0');
                        str << std::fixed << std::dec << c;
                        break;
                    default:
                        break;
                    }
                    break;
                }
                str << " ";
            }
        }
        return str.str();
    }



    inline blocks_t from_string(const string_t &str, byte_view_t view)
    {
        std::istringstream in(str);
        size_t view_size = ((view == byte_view_t::bin) ? 8 : ((view == byte_view_t::dec) ? 3 : 2));
        assert((str.size() % (view_size + 1)) == 0);
        assert(((str.size() % (view_size + 1)) % 8) == 0);
        blocks_t blocks((str.size() / (view_size + 1)) / 8);

        for (size_t i = 0; i < blocks.size(); i++) // blocks
        {
            for (size_t j = 0; j < 8; j++) // bytes
            {
                switch (view)
                {
                case common::byte_view_t::bin:
                    for (size_t k = 0; k < 8; k++)
                    {
                        blocks[i][j * 8 + k] = ('1' == str[(i * 8 + j) * (view_size + 1) + k]);
                    }
                    break;
                default:
                    unsigned int c = 0;
                    switch (view)
                    {
                    case common::byte_view_t::hex:
                        in.width(2);
                        in >> std::fixed >> std::hex >> c;
                        break;
                    case common::byte_view_t::dec:
                        in.width(3);
                        in >> std::fixed >> std::dec >> c;
                        break;
                    default:
                        break;
                    }
                    for (size_t k = 0; k < 8; k++) // bits
                    {
                        blocks[i][j * 8 + k] = ((c >> (8 - k - 1)) & 0x1) == 1;
                    }
                    break;
                }
            }
        }
        return blocks;
    }
}
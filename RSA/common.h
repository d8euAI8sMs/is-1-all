#pragma once

#include <bitset>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <cstdint>

namespace common
{

    using byte_t = unsigned char;
    using int_t = std::uint32_t;
    using long_t = std::uint64_t;

    using ints_t = std::vector < int_t > ;
    using string_t = std::string;

    enum class byte_view_t { hex = 0, dec = 1, bin = 2 };



    inline ints_t to_bstring(const string_t &str)
    {
        ints_t ints(str.length());
        for (size_t i = 0; i < str.length(); i++)
        {
            ints[i] = int_t(str[i]);
        }
        return ints;
    }

    inline string_t to_string(const ints_t &str)
    {
        std::ostringstream os;
        for each (auto c in str)
        {
            os << char(c);
        }
        return os.str();
    }



    inline string_t to_string_view(const ints_t &bytes, byte_view_t view)
    {
        std::ostringstream str;

        for (size_t i = 0; i < bytes.size(); i++) // bytes
        {
            int_t byte = bytes[i];
            switch (view)
            {
            case common::byte_view_t::bin:
                for (size_t j = 0; j < 4; j++)
                {
                    for (size_t k = 0; k < 8; k++) // bits
                    {
                        str << int_t((byte >> ((4 - j - 1) * 8 + (8 - k - 1))) & 0x1);
                    }
                    str << " ";
                }
                break;
            default:
                for (size_t j = 0; j < 4; j++)
                {
                    switch (view)
                    {
                    case common::byte_view_t::hex:
                        str.width(2);
                        str.fill('0');
                        str << std::fixed << std::hex << int_t((byte >> ((4 - j - 1) * 8)) & 0xff);
                        break;
                    case common::byte_view_t::dec:
                        str.width(3);
                        str.fill('0');
                        str << std::fixed << std::dec << int_t((byte >> ((4 - j - 1) * 8)) & 0xff);
                        break;
                    default:
                        break;
                    }
                    str << " ";
                }
                break;
            }
        }
        return str.str();
    }



    inline ints_t from_string_view(const string_t &str, byte_view_t view)
    {
        size_t view_size = ((view == byte_view_t::bin) ? 8 : ((view == byte_view_t::dec) ? 3 : 2));
        assert((str.size() % (view_size + 1)) == 0);

        size_t size = str.size() / (view_size + 1);

        std::istringstream in(str);
        ints_t os(size / 4);

        for (size_t i = 0; i < size / 4; i++) // bytes
        {
            int_t byte = 0;
            switch (view)
            {
            case common::byte_view_t::bin:
                for (size_t j = 0; j < 4; j++)
                {
                    for (size_t k = 0; k < 8; k++)
                    {
                        byte |= (str[i * (view_size + 1) * 4 + (view_size + 1) * j + k] - '0') << ((4 - j - 1) * 8 + (8 - k - 1));
                    }
                }
                os[i] = byte;
                break;
            case common::byte_view_t::hex:
                for (size_t j = 0; j < 4; j++)
                {
                    int_t t;
                    in.width(2);
                    in >> std::fixed >> std::hex >> t;
                    byte |= (t << ((4 - j - 1) * 8));
                }
                break;
            case common::byte_view_t::dec:
                for (size_t j = 0; j < 4; j++)
                {
                    int_t t;
                    in.width(3);
                    in >> std::fixed >> std::dec >> t;
                    byte |= (t << (4 - j - 1) * 8);
                }
                break;
            default:
                break;
            }
            os[i] = byte;
        }
        return os;
    }
}
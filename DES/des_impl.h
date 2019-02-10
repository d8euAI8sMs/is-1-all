#pragma once

#include "common.h"

namespace des
{

    using common::blocks_t;
    using common::block_t;

    namespace
    {

        // initial permutation of input bits (1..64)
        int ip[] = {
            58, 50, 42, 34, 26, 18, 10, 2,
            60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6,
            64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17, 9, 1,
            59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5,
            63, 55, 47, 39, 31, 23, 15, 7
        };

        int final_permutation[] = {
            40, 8, 48, 16, 56, 24, 64, 32,
            39, 7, 47, 15, 55, 23, 63, 31,
            38, 6, 46, 14, 54, 22, 62, 30,
            37, 5, 45, 13, 53, 21, 61, 29,
            36, 4, 44, 12, 52, 20, 60, 28,
            35, 3, 43, 11, 51, 19, 59, 27,
            34, 2, 42, 10, 50, 18, 58, 26,
            33, 1, 41, 9, 49, 17, 57, 25
        };

        // expansion of 32bit R-half to 48bit
        int expansion[] = {
            32, 1, 2, 3, 4, 5, 4, 5,
            6, 7, 8, 9, 8, 9, 10, 11,
            12, 13, 12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21, 20, 21,
            22, 23, 24, 25, 24, 25, 26, 27,
            28, 29, 28, 29, 30, 31, 32, 1
        };

        int s_block[8][4][16] = {
                {{0x0e, 0x04, 0x0d, 0x01, 0x02, 0x0f, 0x0b, 0x08, 0x03, 0x0a, 0x06, 0x0c, 0x05, 0x09, 0x00, 0x07},
                { 0x00, 0x0f, 0x07, 0x04, 0x0e, 0x02, 0x0d, 0x01, 0x0a, 0x06, 0x0c, 0x0b, 0x09, 0x05, 0x03, 0x08 },
                { 0x04, 0x01, 0x04, 0x08, 0x0d, 0x06, 0x02, 0x0b, 0x0f, 0x0c, 0x09, 0x07, 0x03, 0x0a, 0x05, 0x00 },
                { 0x0f, 0x0c, 0x08, 0x02, 0x04, 0x09, 0x01, 0x07, 0x05, 0x0b, 0x03, 0x0e, 0x0a, 0x00, 0x06, 0x0d }},
                { { 0x0f, 0x01, 0x08, 0x0e, 0x06, 0x0b, 0x03, 0x04, 0x09, 0x07, 0x02, 0x0d, 0x0c, 0x00, 0x05, 0x0a },
                { 0x03, 0x0d, 0x04, 0x07, 0x0f, 0x02, 0x08, 0x0e, 0x0c, 0x00, 0x01, 0x0a, 0x06, 0x09, 0x0b, 0x05 },
                { 0x00, 0x0e, 0x07, 0x0b, 0x0a, 0x04, 0x0d, 0x01, 0x05, 0x08, 0x0c, 0x06, 0x09, 0x03, 0x02, 0x0f },
                { 0x0d, 0x08, 0x0a, 0x01, 0x03, 0x0f, 0x04, 0x02, 0x0b, 0x06, 0x07, 0x0c, 0x00, 0x05, 0x0e, 0x09 } },
                { { 0x0a, 0x00, 0x09, 0x0e, 0x06, 0x03, 0x0f, 0x05, 0x01, 0x0d, 0x0c, 0x07, 0x0b, 0x04, 0x02, 0x08 },
                { 0x0d, 0x07, 0x00, 0x09, 0x03, 0x04, 0x06, 0x0a, 0x02, 0x08, 0x05, 0x0e, 0x0c, 0x0b, 0x0f, 0x01 },
                { 0x0d, 0x06, 0x04, 0x09, 0x08, 0x0f, 0x03, 0x00, 0x0b, 0x01, 0x02, 0x0c, 0x05, 0x0a, 0x0e, 0x07 },
                { 0x01, 0x0a, 0x0d, 0x00, 0x06, 0x09, 0x08, 0x07, 0x04, 0x0f, 0x0e, 0x03, 0x0b, 0x05, 0x02, 0x0c } },
                { { 0x07, 0x0d, 0x0e, 0x03, 0x00, 0x06, 0x09, 0x0a, 0x01, 0x02, 0x08, 0x05, 0x0b, 0x0c, 0x04, 0x0f },
                { 0x0d, 0x08, 0x0b, 0x05, 0x06, 0x0f, 0x00, 0x03, 0x04, 0x07, 0x02, 0x0c, 0x01, 0x0a, 0x0e, 0x09 },
                { 0x0a, 0x06, 0x09, 0x00, 0x0c, 0x0b, 0x07, 0x0d, 0x0f, 0x01, 0x03, 0x0e, 0x05, 0x02, 0x08, 0x04 },
                { 0x03, 0x0f, 0x00, 0x06, 0x0a, 0x01, 0x0d, 0x08, 0x09, 0x04, 0x05, 0x0b, 0x0c, 0x07, 0x02, 0x0e } },
                { { 0x02, 0x0c, 0x04, 0x01, 0x07, 0x0a, 0x0b, 0x06, 0x08, 0x05, 0x03, 0x0f, 0x0d, 0x00, 0x0e, 0x09 },
                { 0x0e, 0x0b, 0x02, 0x0c, 0x04, 0x07, 0x0d, 0x01, 0x05, 0x00, 0x0f, 0x0a, 0x03, 0x09, 0x08, 0x06 },
                { 0x04, 0x02, 0x01, 0x0b, 0x0a, 0x0d, 0x07, 0x08, 0x0f, 0x09, 0x0c, 0x05, 0x06, 0x03, 0x00, 0x0e },
                { 0x0b, 0x08, 0x0c, 0x07, 0x01, 0x0e, 0x02, 0x0d, 0x06, 0x0f, 0x00, 0x09, 0x0a, 0x04, 0x05, 0x03 } },
                { { 0x0c, 0x01, 0x0a, 0x0f, 0x09, 0x02, 0x06, 0x08, 0x00, 0x0d, 0x03, 0x04, 0x0e, 0x07, 0x05, 0x0b },
                { 0x0a, 0x0f, 0x04, 0x02, 0x07, 0x0c, 0x09, 0x05, 0x06, 0x01, 0x0d, 0x0e, 0x00, 0x0b, 0x03, 0x08 },
                { 0x09, 0x0e, 0x0f, 0x05, 0x02, 0x08, 0x0c, 0x03, 0x07, 0x00, 0x04, 0x0a, 0x01, 0x0d, 0x01, 0x06 },
                { 0x04, 0x03, 0x02, 0x0c, 0x09, 0x05, 0x0f, 0x0a, 0x0b, 0x0e, 0x01, 0x07, 0x06, 0x00, 0x08, 0x0d } },
                { { 0x04, 0x0b, 0x02, 0x0e, 0x0f, 0x00, 0x08, 0x0d, 0x03, 0x0c, 0x09, 0x07, 0x05, 0x0a, 0x06, 0x01 },
                { 0x0d, 0x00, 0x0b, 0x07, 0x04, 0x09, 0x01, 0x0a, 0x0e, 0x03, 0x05, 0x0c, 0x02, 0x0f, 0x08, 0x06 },
                { 0x01, 0x04, 0x0b, 0x0d, 0x0c, 0x03, 0x07, 0x0e, 0x0a, 0x0f, 0x06, 0x08, 0x00, 0x05, 0x09, 0x02 },
                { 0x06, 0x0b, 0x0d, 0x08, 0x01, 0x04, 0x0a, 0x07, 0x09, 0x05, 0x00, 0x0f, 0x0e, 0x02, 0x03, 0x0c } },
                { { 0x0d, 0x02, 0x08, 0x04, 0x06, 0x0f, 0x0b, 0x01, 0x0a, 0x09, 0x03, 0x0e, 0x05, 0x00, 0x0c, 0x07 },
                { 0x01, 0x0f, 0x0d, 0x08, 0x0a, 0x03, 0x07, 0x04, 0x0c, 0x05, 0x06, 0x0b, 0x00, 0x0e, 0x09, 0x02 },
                { 0x07, 0x0b, 0x04, 0x01, 0x09, 0x0c, 0x0e, 0x02, 0x00, 0x06, 0x0a, 0x0d, 0x0f, 0x03, 0x05, 0x08 },
                { 0x02, 0x01, 0x0e, 0x07, 0x04, 0x0a, 0x08, 0x0d, 0x0f, 0x0c, 0x09, 0x00, 0x03, 0x05, 0x06, 0x0b } }};

        int permutation_func[] = {
            16, 7, 20, 21, 29, 12, 28, 17,
            1, 15, 23, 26, 5, 18, 31, 10,
            2, 8, 24, 14, 32, 27, 3, 9,
            19, 13, 30, 6, 22, 11, 4, 25
        };

        int key_ip[] = {
            57, 49, 41, 33, 25, 17, 9,
            1, 58, 50, 42, 34, 26, 18,
            10, 2, 59, 51, 43, 35, 27,
            19, 11, 3, 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15,
            7, 62, 54, 46, 38, 30, 22,
            14, 6, 61, 53, 45, 37, 29,
            21, 13, 5, 28, 20, 12, 4
        };

        int n_shifts[] = {
            1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
        };

        int key_fp[] = {
            14, 17, 11, 24, 1, 5,
            3, 28, 15, 6, 21, 10,
            23, 19, 12, 4, 26, 8,
            16, 7, 27, 20, 13, 2,
            41, 52, 31, 37, 47, 55,
            30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53,
            46, 42, 50, 36, 29, 32
        };

        void do_initial_permutation(block_t &input  /* 64bit */,
                                    block_t &output /* 64bit */)
        {
            for (size_t i = 0; i < 64; i++)
            {
                output[i] = input[ip[i] - 1];
            }
        }

        void do_final_permutation(block_t &input  /* 64bit */,
                                  block_t &output /* 64bit */)
        {
            for (size_t i = 0; i < 64; i++)
            {
                output[i] = input[final_permutation[i] - 1];
            }
        }

        void do_f_function(block_t &input  /* 32bit */,
                           block_t &output /* 32bit */,
                           block_t &key    /* 48bit */)
        {
            block_t expanded; /* 48bit */
            for (size_t i = 0; i < 48; i++)
            {
                expanded[i] = input[expansion[i] - 1] ^ key[i];
            }
            for (size_t i = 0; i < 8; i++) // E(R) = B1...B8, Bi -- 6bit
            {
                // Bi = b0b1b2b3b4b5;
                // a  = b0b5 (int), b = b1b2b3b4 (int)
                // c  = s_block[i][a][b] = b'0b'1b'2b'3 (4bit)
                int a = ((expanded[i * 6 + 0] << 1) | (expanded[i * 6 + 5] << 0));
                int b = (
                    (expanded[i * 6 + 1] << 3) |
                    (expanded[i * 6 + 2] << 2) |
                    (expanded[i * 6 + 3] << 1) |
                    (expanded[i * 6 + 4] << 0)
                );
                int c = s_block[i][a][b];
                for (size_t j = 0; j < 4; j++)
                {
                    output[i * 4 + j] = (((c >> (4 - j - 1)) & 0x1) == 1);
                }
            }
            for (size_t i = 0; i < 32; i++)
            {
                expanded[i] = output[permutation_func[i] - 1];
            }
            output = expanded;
        }

        void normalize_key(block_t &input  /* 64bit */,
                           block_t &output /* 56bit */)
        {
            for (size_t i = 0; i < 56; i++)
            {
                output[i] = input[key_ip[i] - 1];
            }
        }

        void shift_key(block_t &input /* 56bit */,
                       size_t n_shifts)
        {
            input >>= n_shifts;
            for (size_t i = 0; i < n_shifts; i++)
            {
                input[56 - i - 1] = input[64 - i - 1];
            }
        }

        void get_key(block_t &input  /* 56bit */,
                     block_t &output /* 48bit */)
        {
            for (size_t i = 0; i < 48; i++)
            {
                output[i] = input[key_fp[i] - 1];
            }
        }


        blocks_t get_keys(block_t &key)
        {
            blocks_t keys(16);
            block_t normalized;
            normalize_key(key, normalized);
            for (size_t fp = 0; fp < 16; fp++)
            {
                shift_key(normalized, n_shifts[fp]);
                get_key(normalized, keys[fp]);
            }
            return keys;
        }


        void do_encrypt_block(block_t  &input  /* 64bit */,
                              block_t  &output /* 64bit */,
                              blocks_t &keys   /* 16x48bit */)
        {
            block_t left, right;
            block_t left_prev, right_prev;
            for (size_t i = 0; i < 32; i++)
            {
                left_prev[i] = input[i];
                right_prev[i] = input[32 + i];
            }
            for (size_t fp = 0; fp < 16; fp++)
            {
                left = right_prev;
                do_f_function(right_prev, output, keys[fp]);
                right = left_prev ^ output;
                left_prev = left;
                right_prev = right;
            }
            for (size_t i = 0; i < 32; i++)
            {
                output[i] = right_prev[i];
                output[32 + i] = left_prev[i];
            }
        }

        void do_decrypt_block(block_t  &input  /* 64bit */,
                              block_t  &output /* 64bit */,
                              blocks_t &keys   /* 16x48bit */)
        {
            block_t left, right;
            block_t left_prev, right_prev;
            for (size_t i = 0; i < 32; i++)
            {
                right_prev[i] = input[i];
                left_prev[i] = input[32 + i];
            }
            for (size_t fp = 16; fp-- > 0;)
            {
                right = left_prev;
                do_f_function(left_prev, output, keys[fp]);
                left = right_prev ^ output;
                left_prev = left;
                right_prev = right;
            }
            for (size_t i = 0; i < 32; i++)
            {
                output[i] = left_prev[i];
                output[32 + i] = right_prev[i];
            }
        }
    }

    blocks_t encrypt(blocks_t &input, block_t &key)
    {
        blocks_t output(input.size());
        blocks_t keys = get_keys(key);
        block_t tmp;
        for (size_t block = 0; block < input.size(); block++)
        {
            do_initial_permutation(input[block], output[block]);
            do_encrypt_block(output[block], tmp, keys);
            do_final_permutation(tmp, output[block]);
        }
        return output;
    }

    blocks_t decrypt(blocks_t &input, block_t &key)
    {
        blocks_t output(input.size());
        blocks_t keys = get_keys(key);
        block_t tmp;
        for (size_t block = 0; block < input.size(); block++)
        {
            do_initial_permutation(input[block], output[block]);
            do_decrypt_block(output[block], tmp, keys);
            do_final_permutation(tmp, output[block]);
        }
        return output;
    }
}
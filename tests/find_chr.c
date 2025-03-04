/*
MIT License

Copyright (c) 2023 MetaReal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#include "tlib.h"

#define MRFSTR_TLIB_CONFIG MRFSTR_CONFIG_FUNC_SEARCH_CHR

#define MRFSTR_TLIB_OBJ(size)                                 \
    do                                                        \
    {                                                         \
        MRFSTR_SIZE(str) = size;                              \
                                                              \
        if (first)                                            \
            obj = mrfstr_find_chr(str, '1') == MRFSTR_INVIDX; \
        else                                                  \
        {                                                     \
            mrfstr_size_t idx;                                \
            mrfstr_ubyte_t i;                                 \
                                                              \
            for (i = 0; i != 100; i++)                        \
            {                                                 \
                idx = MRFSTR_GENERATE_RAND % size;            \
                                                              \
                MRFSTR_DATA(str)[idx] = '1';                  \
                obj = mrfstr_find_chr(str, '1') == idx;       \
                MRFSTR_DATA(str)[idx] = '0';                  \
                                                              \
                if (!obj)                                     \
                    break;                                    \
            }                                                 \
        }                                                     \
    } while (0)

#define MRFSTR_TLIB_FREE mrfstr_free(str)

int main(void)
{
    mrfstr_t str;
    mrfstr_bool_t first;

    mrfstr_config(MRFSTR_CONFIG_LEVEL_EXTREME);
    MRFSTR_TLIB_INIT(str,);
    MRFSTR_TLIB_MEMSET(str, '0', MRFSTR_TEST4_SIZE);

    if (mrfstr_config_func(MRFSTR_TLIB_CONFIG, MRFSTR_CONFIG_SIMD_AVX512, MRFSTR_CONFIG_SIMD_AVX512) == MRFSTR_RES_NOERROR)
    {
        first = MRFSTR_TRUE;
        MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
        first = MRFSTR_FALSE;
        MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
    }

    if (mrfstr_config_func(MRFSTR_TLIB_CONFIG, MRFSTR_CONFIG_SIMD_AVX, MRFSTR_CONFIG_SIMD_AVX) == MRFSTR_RES_NOERROR)
    {
        first = MRFSTR_TRUE;
        MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
        first = MRFSTR_FALSE;
        MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
    }

    if (mrfstr_config_func(MRFSTR_TLIB_CONFIG, MRFSTR_CONFIG_SIMD_SSE, MRFSTR_CONFIG_SIMD_SSE) == MRFSTR_RES_NOERROR)
    {
        first = MRFSTR_TRUE;
        MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
        first = MRFSTR_FALSE;
        MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
        MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
    }

    mrfstr_config_func(MRFSTR_TLIB_CONFIG, MRFSTR_CONFIG_SIMD_INT64, MRFSTR_CONFIG_SIMD_INT64);

    first = MRFSTR_TRUE;
    MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
    MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
    MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
    MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);
    first = MRFSTR_FALSE;
    MRFSTR_TLIB_ROUND(MRFSTR_TEST1_SIZE);
    MRFSTR_TLIB_ROUND(MRFSTR_TEST2_SIZE);
    MRFSTR_TLIB_ROUND(MRFSTR_TEST3_SIZE);
    MRFSTR_TLIB_ROUND(MRFSTR_TEST4_SIZE);

    MRFSTR_TLIB_FREE;
    return EXIT_SUCCESS;
}

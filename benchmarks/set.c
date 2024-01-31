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

#include "blib.h"

#define MRFSTR_BLIB_CONFIG MRFSTR_CONFIG_TYPE_MEMORY

#define MRFSTR_BLIB_CSTR_PRE(size)
#define MRFSTR_BLIB_CSTR(size) memcpy(cdst, csrc, size)
#define MRFSTR_BLIB_CSTR_POST(size)

#define MRFSTR_BLIB_PRE(size) MRFSTR_SIZE(src) = size
#define MRFSTR_BLIB_OBJ(size) mrfstr_set(dst, src)
#define MRFSTR_BLIB_POST(size)

int main(void)
{
    mrfstr_t src, dst;
    mrfstr_data_t csrc, cdst;
#ifdef _WIN32
    LARGE_INTEGER freq;
#endif
    mrfstr_double_t benchmark[7];

    MRFSTR_BLIB_FIRST;

    MRFSTR_BLIB_INIT_STR(csrc,);
    MRFSTR_BLIB_INIT_STR(cdst, free(csrc));
    memset(csrc, '0', mrfstr_test_sizes[6]);

    MRFSTR_BLIB_ROUND_CSTR;
    free(csrc);
    free(cdst);

    MRFSTR_BLIB_INIT(src,);
    MRFSTR_BLIB_INIT(dst, mrfstr_free(src));
    mrfstr_repeat_chr(src, '0', mrfstr_test_sizes[6]);

    mrfstr_config(MRFSTR_BLIB_CONFIG,
        MRFSTR_CONFIG_SIMD_AVX512, MRFSTR_CONFIG_SIMD_AVX512);
    MRFSTR_BLIB_ROUND("AVX512");

    mrfstr_config(MRFSTR_BLIB_CONFIG,
        MRFSTR_CONFIG_SIMD_AVX, MRFSTR_CONFIG_SIMD_AVX);
    MRFSTR_BLIB_ROUND("AVX   ");

    mrfstr_config(MRFSTR_BLIB_CONFIG,
        MRFSTR_CONFIG_SIMD_SSE, MRFSTR_CONFIG_SIMD_SSE);
    MRFSTR_BLIB_ROUND("SSE   ");

    mrfstr_config(MRFSTR_BLIB_CONFIG,
        MRFSTR_CONFIG_SIMD_NONE, MRFSTR_CONFIG_SIMD_NONE);
    MRFSTR_BLIB_ROUND("INT64 ");

    mrfstr_free(src);
    mrfstr_free(dst);
    return 0;
}

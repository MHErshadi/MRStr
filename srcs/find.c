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

#include <mrfstr-intern.h>

mrfstr_idx_t mrfstr_find_chr(
    mrfstr_ct str, mrfstr_chr_t chr)
{
    if (!MRFSTR_SIZE(str))
        return MRFSTR_INVIDX;

    return __mrfstr_findchr(MRFSTR_DATA(str), chr, MRFSTR_SIZE(str));
}

mrfstr_idx_t mrfstr_n_find_chr(
    mrfstr_ct str, mrfstr_chr_t chr, mrfstr_size_t size)
{
    if (size > MRFSTR_SIZE(str))
        size = MRFSTR_SIZE(str);

    if (!MRFSTR_SIZE(str))
        return MRFSTR_INVIDX;

    return __mrfstr_findchr(MRFSTR_DATA(str), chr, size);
}

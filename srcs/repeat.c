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
#include <string.h>

mrfstr_res_enum_t mrfstr_repeat(
    mrfstr_t res, mrfstr_ct str,
    mrfstr_size_t count)
{
    if (MRFSTR_SIZE(str) == 1)
    {
        mrfstr_repeat_chr(res, *MRFSTR_DATA(str), count);
        return MRFSTR_RES_NOERROR;
    }

    if (!(count && MRFSTR_SIZE(str)))
    {
        MRFSTR_SIZE(res) = 0;
        return MRFSTR_RES_NOERROR;
    }

    if (count == 1)
    {
        if (res == str)
            return MRFSTR_RES_NOERROR;

        __mrfstr_copy(MRFSTR_DATA(res), MRFSTR_DATA(str), MRFSTR_SIZE(str));
        MRFSTR_SIZE(res) = MRFSTR_SIZE(str);
        return MRFSTR_RES_NOERROR;
    }

    if (res == str)
    {
        mrfstr_size_t size = MRFSTR_SIZE(res) * count;
        if (size / count != MRFSTR_SIZE(res))
            return MRFSTR_RES_OVERFLOW_ERROR;

        while (MRFSTR_SIZE(res) <= size - MRFSTR_SIZE(res))
        {
            __mrfstr_copy(MRFSTR_DATA(res) + MRFSTR_SIZE(res), MRFSTR_DATA(res), MRFSTR_SIZE(res));
            MRFSTR_SIZE(res) <<= 1;
        }

        MRFSTR_SIZE(res) = size - MRFSTR_SIZE(res);
        if (MRFSTR_SIZE(res))
            __mrfstr_copy(MRFSTR_DATA(res) + MRFSTR_SIZE(res), MRFSTR_DATA(res), MRFSTR_SIZE(res));

        MRFSTR_SIZE(res) = size;
        return MRFSTR_RES_NOERROR;
    }

    mrfstr_size_t size = MRFSTR_SIZE(str) * count;
    if (size / count != MRFSTR_SIZE(str))
        return MRFSTR_RES_OVERFLOW_ERROR;

    __mrfstr_copy(MRFSTR_DATA(res), MRFSTR_DATA(str), MRFSTR_SIZE(str));
    MRFSTR_SIZE(res) = MRFSTR_SIZE(str);

    while (MRFSTR_SIZE(res) <= size - MRFSTR_SIZE(res))
    {
        __mrfstr_copy(MRFSTR_DATA(res) + MRFSTR_SIZE(res), MRFSTR_DATA(res), MRFSTR_SIZE(res));
        MRFSTR_SIZE(res) <<= 1;
    }

    MRFSTR_SIZE(res) = size - MRFSTR_SIZE(res);
    if (MRFSTR_SIZE(res))
        __mrfstr_copy(MRFSTR_DATA(res) + MRFSTR_SIZE(res), MRFSTR_DATA(res), MRFSTR_SIZE(res));

    MRFSTR_SIZE(res) = size;
    return MRFSTR_RES_NOERROR;
}

void mrfstr_repeat_chr(
    mrfstr_t res, mrfstr_chr_t chr,
    mrfstr_size_t count)
{
    MRFSTR_SIZE(res) = count;
    if (count)
        __mrfstr_fill(MRFSTR_DATA(res), chr, count);
}

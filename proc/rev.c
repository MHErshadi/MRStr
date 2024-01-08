/*
    MRFStr Library version 0.1.0
    MetaReal Fast String Library
*/

#include <mrfstr-intern.h>

#define mrfstr_rev_rem  \
    for (; rem; rem--)  \
    {                   \
        chr = *--right; \
        *right = *str;  \
        *str++ = chr;   \
    }

#define mrfstr_rev2_rem \
    for (; rem; rem--)  \
        *left++ = *--right

#pragma pack(push, 1)
struct __MRFSTR_REV_T
{
    mrfstr_data_t left;
    mrfstr_data_t right;
    mrfstr_size_t size;
};
#pragma pack(pop)
typedef struct __MRFSTR_REV_T *mrfstr_rev_t;

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
mrfstr_ptr_t __mrfstr_rev_threaded(
    mrfstr_ptr_t args);
#elif defined(_WIN32)
DWORD WINAPI __mrfstr_rev_threaded(
    LPVOID args);
#endif

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
mrfstr_ptr_t __mrfstr_rev2_threaded(
    mrfstr_ptr_t args);
#elif defined(_WIN32)
DWORD WINAPI __mrfstr_rev2_threaded(
    LPVOID args);
#endif

void __mrfstr_rev(
    mrfstr_data_t str, mrfstr_size_t size)
{
    mrfstr_data_t right = str + size;
    if (size < MRFSTR_SLIMIT)
    {
        mrfstr_chr_t chr;
        while (str < right)
        {
            chr = *--right;
            *right = *str;
            *str++ = chr;
        }
        return;
    }

    mrfstr_byte_t revsize = _mrfstr_config.nrev_size << 1;
    if (_mrfstr_config.tcount == 1 || size < MRFSTR_TLIMIT)
    {
        mrfstr_byte_t rem = (uintptr_t)str % revsize;
        mrfstr_chr_t chr;
        if (rem)
        {
            rem = revsize - rem;
            size -= rem;
            mrfstr_rev_rem;
        }

        rem = size % revsize;
        size -= rem;

        _mrfstr_config.nrev_sub(str, right, size / revsize);
        str += size;
        right -= size;

        while (str < right)
        {
            chr = *--right;
            *right = *str;
            *str++ = chr;
        }
        return;
    }

    mrfstr_byte_t tcount;
    if (size > _mrfstr_config.tcount * MRFSTR_TSIZE)
        tcount = _mrfstr_config.tcount;
    else
        tcount = (mrfstr_byte_t)(size / MRFSTR_TSIZE);

    revsize = _mrfstr_config.trev_size << 1;
    mrfstr_short_t rem = (uintptr_t)str & revsize;
    mrfstr_chr_t chr;
    if (rem)
    {
        rem = revsize - rem;
        size -= rem;
        mrfstr_rev_rem;
    }

    mrfstr_short_t factor = revsize * tcount;
    rem = size % factor;
    mrfstr_size_t inc = (size /= factor) * revsize;

    mrfstr_byte_t nthreads = tcount - 1;
    mrfstr_thread_t *threads = malloc(nthreads * sizeof(mrfstr_thread_t));
    mrfstr_byte_t i = 0;
    if (threads)
    {
        mrfstr_rev_t data;
        for (i = 0; i != nthreads; i++)
        {
            data = malloc(sizeof(struct __MRFSTR_REV_T));
            if (!data)
                break;

            data->left = str;
            data->right = right;
            data->size = size;

            str += inc;
            right -= inc;

            mrfstr_create_thread(__mrfstr_rev_threaded)
            {
                str -= inc;
                right += inc;
                free(data);
                break;
            }
        }

        tcount -= i;
    }

    _mrfstr_config.trev_sub(str, right, size * tcount);

    inc *= tcount;
    str += inc;
    right -= inc;
    while (str < right)
    {
        chr = *--right;
        *right = *str;
        *str++ = chr;
    }

    mrfstr_close_threads;
    free(threads);
}

void __mrfstr_rev2(
    mrfstr_data_t left, mrfstr_data_ct right, mrfstr_size_t size)
{
    if (size < MRFSTR_SLIMIT)
    {
        while (left < right)
            *left++ = *--right;
        return;
    }

    if (_mrfstr_config.tcount == 1 || size < MRFSTR_TLIMIT)
    {
        mrfstr_byte_t rem = (uintptr_t)left % _mrfstr_config.nrev_size;
        if (rem)
        {
            rem = _mrfstr_config.nrev_size - rem;
            size -= rem;
            mrfstr_rev2_rem;
        }

        rem = size % _mrfstr_config.nrev_size;
        size -= rem;

        _mrfstr_config.nrev2_sub(left, right, size / _mrfstr_config.nrev_size);
        left += size;
        right -= size;

        while (left < right)
            *left++ = *--right;
        return;
    }

    mrfstr_byte_t tcount;
    if (size > _mrfstr_config.tcount * MRFSTR_TSIZE)
        tcount = _mrfstr_config.tcount;
    else
        tcount = (mrfstr_byte_t)(size / MRFSTR_TSIZE);

    mrfstr_short_t rem = (uintptr_t)left & _mrfstr_config.trev_size;
    if (rem)
    {
        rem = _mrfstr_config.trev_size - rem;
        size -= rem;
        mrfstr_rev2_rem;
    }

    mrfstr_short_t factor = _mrfstr_config.trev_size * tcount;
    rem = size % factor;
    mrfstr_size_t inc = (size /= factor) * _mrfstr_config.trev_size;

    mrfstr_byte_t nthreads = tcount - 1;
    mrfstr_thread_t *threads = malloc(nthreads * sizeof(mrfstr_thread_t));
    mrfstr_byte_t i = 0;
    if (threads)
    {
        mrfstr_rev_t data;
        for (i = 0; i != nthreads; i++)
        {
            data = malloc(sizeof(struct __MRFSTR_REV_T));
            if (!data)
                break;

            data->left = left;
            data->right = (mrfstr_data_t)right;
            data->size = size;

            left += inc;
            right -= inc;

            mrfstr_create_thread(__mrfstr_rev2_threaded)
            {
                left += inc;
                right -= inc;
                free(data);
                break;
            }
        }

        tcount -= i;
    }

    _mrfstr_config.trev2_sub(left, right, size * tcount);

    inc *= tcount;
    left += inc;
    right -= inc;
    while (left < right)
        *left++ = *--right;

    mrfstr_close_threads;
    free(threads);
}

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
mrfstr_ptr_t __mrfstr_rev_threaded(
    mrfstr_ptr_t args)
#elif defined(_WIN32)
DWORD WINAPI __mrfstr_rev_threaded(
    LPVOID args)
#endif
{
    mrfstr_rev_t data = (mrfstr_rev_t)args;
    _mrfstr_config.trev_sub(data->left, data->right, data->size);

    free(data);
    return MRFSTR_TFUNC_RET;
}

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
mrfstr_ptr_t __mrfstr_rev2_threaded(
    mrfstr_ptr_t args)
#elif defined(_WIN32)
DWORD WINAPI __mrfstr_rev2_threaded(
    LPVOID args)
#endif
{
    mrfstr_rev_t data = (mrfstr_rev_t)args;
    _mrfstr_config.trev2_sub(data->left, data->right, data->size);

    free(data);
    return MRFSTR_TFUNC_RET;
}

/*
    MRFStr Library version 1.0.0
    MetaReal Fast String Library
*/

#include <mrfstr-intern.h>
#include <alloc.h>
#include <string.h>

#ifdef __AVX512F__

typedef __m512i mrfstr_memset_simd_t;
#define MRFSTR_MEMSET_SIMD_SIZE 64
#define MRFSTR_MEMSET_SIMD_SHIFT 6

#define mrfstr_memset_set _mm512_set1_epi8
#define mrfstr_memset_store _mm512_stream_si512

#elif defined(__AVX__)

typedef __m256i mrfstr_memset_simd_t;
#define MRFSTR_MEMSET_SIMD_SIZE 32
#define MRFSTR_MEMSET_SIMD_SHIFT 5

#define mrfstr_memset_set _mm256_set1_epi8
#define mrfstr_memset_store _mm256_stream_si256

#elif defined(__SSE2__)

typedef __m128i mrfstr_memset_simd_t;
#define MRFSTR_MEMSET_SIMD_SIZE 16
#define MRFSTR_MEMSET_SIMD_SHIFT 4

#define mrfstr_memset_set _mm_set1_epi8
#define mrfstr_memset_store _mm_stream_si128

#else
#define MRFSTR_MEMSET_NOSIMD

typedef mrfstr_chr_t mrfstr_memset_simd_t;
#define MRFSTR_MEMSET_SIMD_SIZE 1

#endif

#ifdef MRFSTR_MEMSET_NOSIMD
#define MRFSTR_MEMSET_SLIMIT (0x800 * MRFSTR_MEMSET_SIMD_SIZE - 1)
#else
#define MRFSTR_MEMSET_SIMD_MASK (MRFSTR_MEMSET_SIMD_SIZE - 1)
#define MRFSTR_MEMSET_SLIMIT (0x100 * MRFSTR_MEMSET_SIMD_SIZE - 1)

#define mrfstr_memset_sub(r, c, s)     \
    for (; s; r++, s--)                \
        mrfstr_memset_store(r, block)

#endif

#if MRFSTR_THREADING
#include <pthread.h>

#define MRFSTR_MEMSET_TCHK (MRFSTR_MEMSET_SIMD_SIZE * MRFSTR_THREAD_COUNT)

#ifdef MRFSTR_MEMSET_NOSIMD
#define MRFSTR_MEMSET_TLIMIT (0x80000 * MRFSTR_THREAD_COUNT - 1)
#else
#define MRFSTR_MEMSET_TLIMIT (0x10000 * MRFSTR_MEMSET_TCHK - 1)
#endif

struct __MRFSTR_MEMSET_T
{
    mrfstr_memset_simd_t *res;
    mrfstr_chr_t chr;
    mrfstr_size_t size;
};
typedef struct __MRFSTR_MEMSET_T *mrfstr_memset_t;

void *mrfstr_memset_threaded(void *args);
#endif

void mrfstr_memset(mrfstr_data_t res, mrfstr_chr_t chr, mrfstr_size_t size)
{
    if (size <= MRFSTR_MEMSET_SLIMIT)
    {
        memset(res, chr, size);
        return;
    }

#ifndef MRFSTR_MEMSET_NOSIMD
    mrfstr_bit_t align = (uintptr_t)res & MRFSTR_MEMSET_SIMD_MASK;
    if (align)
    {
        align = MRFSTR_MEMSET_SIMD_SIZE - align;
        memset(res, chr, align);

        res += align;
        size -= align;
    }
#endif

#if !defined(MRFSTR_MEMSET_NOSIMD) || MRFSTR_THREADING
    mrfstr_memset_simd_t *rblock = (mrfstr_memset_simd_t*)res;
#endif

#if MRFSTR_THREADING
    if (size <= MRFSTR_MEMSET_TLIMIT)
    {
#endif
#ifdef MRFSTR_MEMSET_NOSIMD
        memset(res, chr, size);
#else
        mrfstr_size_t rem = size & MRFSTR_MEMSET_SIMD_MASK;
        size >>= MRFSTR_MEMSET_SIMD_SHIFT;

        mrfstr_memset_simd_t block = mrfstr_memset_set(chr);
        mrfstr_memset_sub(rblock, block, size);
        memset(rblock, chr, rem);
#endif
#if MRFSTR_THREADING
        return;
    }

    mrfstr_size_t rem = size % MRFSTR_MEMSET_TCHK;
    size /= MRFSTR_MEMSET_TCHK;

    pthread_t threads[MRFSTR_THREAD_COUNT];
    mrfstr_bit_t i;
    mrfstr_memset_t data;
    for (i = 0; i < MRFSTR_THREAD_COUNT; i++)
    {
        data = mrstr_alloc(sizeof(struct __MRFSTR_MEMSET_T));
        if (!data)
            goto rem;

        data->res = rblock;
        data->chr = chr;
        data->size = size;

        rblock += size;

        if (pthread_create(threads + i, NULL, mrfstr_memset_threaded, data))
        {
            rblock -= size;

            mrstr_free(data);
            goto rem;
        }
    }

    memset(rblock, chr, rem);

ret:
    while (i)
        pthread_join(threads[--i], NULL);
    return;

rem:
    size *= MRFSTR_THREAD_COUNT - i;

#ifdef MRFSTR_MEMSET_NOSIMD
    memset(rblock, chr, size + rem);
#else
    mrfstr_memset_simd_t block = mrfstr_memset_set(chr);
    mrfstr_memset_sub(rblock, block, size);
    memset(rblock, chr, rem);
#endif

    goto ret;
#endif
}

#if MRFSTR_THREADING
void *mrfstr_memset_threaded(void *args)
{
    mrfstr_memset_t data = (mrfstr_memset_t)args;

#ifdef MRFSTR_MEMSET_NOSIMD
    memset(data->res, data->chr, data->size);
#else
    mrfstr_memset_simd_t block = mrfstr_memset_set(data->chr);
    mrfstr_memset_sub(data->res, block, data->size);
#endif

    mrstr_free(data);
    return NULL;
}
#endif

/*
    MRFStr Library version 0.1.0
    MetaReal Fast String Library
*/

#ifndef __MRFSTR_INTERN__
#define __MRFSTR_INTERN__

#include <mrfstr.h>
#include <immintrin.h>

#ifdef _MSC_VER
#include <simddef.h>
#endif

#ifdef __AVX512F__

typedef __m512i mrfstr_simd_t;
#define MRFSTR_SIMD_SIZE 64
#define MRFSTR_SIMD_SHIFT 6

#define mrfstr_simd_set _mm512_set1_epi8
#define mrfstr_simd_loadu _mm512_loadu_si512
#define mrfstr_simd_storeu _mm512_storeu_si512
#define mrfstr_simd_stream _mm512_stream_si512

#elif defined(__AVX__)

typedef __m256i mrfstr_simd_t;
#define MRFSTR_SIMD_SIZE 32
#define MRFSTR_SIMD_SHIFT 5

#define mrfstr_simd_set _mm256_set1_epi8
#define mrfstr_simd_loadu _mm256_loadu_si256
#define mrfstr_simd_storeu _mm256_storeu_si256
#define mrfstr_simd_stream _mm256_stream_si256

#elif defined(__SSE2__)

typedef __m128i mrfstr_simd_t;
#define MRFSTR_SIMD_SIZE 16
#define MRFSTR_SIMD_SHIFT 4

#define mrfstr_simd_set _mm_set1_epi8
#define mrfstr_simd_loadu _mm_loadu_si128
#define mrfstr_simd_storeu _mm_storeu_si128
#define mrfstr_simd_stream _mm_stream_si128

#else
#define MRFSTR_NOSIMD

typedef uint64_t mrfstr_simd_t;
#define MRFSTR_SIMD_SIZE 8
#define MRFSTR_SIMD_SHIFT 3

#define mrfstr_simd_loadu(x) (*(x))
#define mrfstr_simd_storeu(x, y) (*(x) = (y))
#define mrfstr_simd_stream(x, y) (*(x) = (y))

#endif

#define MRFSTR_SIMD_MASK (MRFSTR_SIMD_SIZE - 1)
#define MRFSTR_SIMD_SLIMIT (0x100 * MRFSTR_SIMD_SIZE)

#if MRFSTR_THREAD_COUNT
#define MRFSTR_SIMD_TCHK (MRFSTR_SIMD_SIZE * MRFSTR_THREAD_COUNT)
#define MRFSTR_SIMD_TLIMIT (0x10000ULL * MRFSTR_SIMD_TCHK)
#endif

#if MRFSTR_THREAD_COUNT
#if defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>

typedef pthread_t mrfstr_thread_t;
typedef pthread_mutex_t mrfstr_mutex_t;
typedef mrfstr_mutex_t *mrfstr_mutex_p;

#define MRFSTR_TFUNC_RET NULL
#define MRFSTR_CAST_MUTEX(m) (&(m))

#define mrfstr_create_thread(f) \
    if (pthread_create(threads + i, NULL, (f), data))

#define mrfstr_close_threads \
    while (i)                \
        pthread_join(threads[--i], NULL)

#define mrfstr_create_mutex(m) \
    if (pthread_mutex_init(&(m), NULL))

#define mrfstr_close_mutex(m) pthread_mutex_destroy(&(m))
#define mrfstr_lock_mutex(m) pthread_mutex_lock(m)
#define mrfstr_unlock_mutex(m) pthread_mutex_unlock(m)

#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef HANDLE mrfstr_thread_t;
typedef HANDLE mrfstr_mutex_t;
typedef HANDLE mrfstr_mutex_p;

#define MRFSTR_TFUNC_RET TRUE
#define MRFSTR_CAST_MUTEX(m) (m)

#define mrfstr_create_thread(f)                             \
    threads[i] = CreateThread(NULL, 0, (f), data, 0, NULL); \
    if (!threads[i])

#define mrfstr_close_threads                                              \
    WaitForMultipleObjects(MRFSTR_THREAD_COUNT, threads, TRUE, INFINITE); \
    while (i)                                                             \
        CloseHandle(threads[--i])

#define mrfstr_create_mutex(m)            \
    (m) = CreateMutex(NULL, FALSE, NULL); \
    if (!(m))

#define mrfstr_close_mutex CloseHandle
#define mrfstr_lock_mutex(m) WaitForSingleObject((m), INFINITE)
#define mrfstr_unlock_mutex ReleaseMutex

#else
#error Your OS is not supported yet for multithreading
#endif
#endif

void mrfstr_memcpy(mrfstr_data_t dst, mrfstr_data_ct src, mrfstr_size_t size);
void mrfstr_memset(mrfstr_data_t res, mrfstr_chr_t chr, mrfstr_size_t size);

mrfstr_bool_t mrfstr_memcmp(mrfstr_data_ct str1, mrfstr_data_ct str2, mrfstr_size_t size);
mrfstr_bool_t mrfstr_memchr(mrfstr_data_ct str, mrfstr_chr_t chr, mrfstr_size_t size);
mrfstr_idx_t mrfstr_memchr2(mrfstr_data_ct str, mrfstr_chr_t chr, mrfstr_size_t size);

#endif /* __MRFSTR_INTERN__ */

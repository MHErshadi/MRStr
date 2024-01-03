/*
    MRStr Library version 0.1.0
    MetaReal String Library
*/

#ifndef __MRSTR_ALLOC__
#define __MRSTR_ALLOC__

#include <stdint.h>

typedef uint64_t mrstr_size_t;
typedef void *mrstr_ptr_t;

#ifndef __MRFSTR_DEBUG__

inline mrstr_ptr_t mrstr_alloc(mrstr_size_t size)
{
    return malloc((size_t)size);
}

inline mrstr_ptr_t mrstr_realloc(mrstr_ptr_t block, mrstr_size_t size)
{
    return realloc(block, (size_t)size);
}

inline void mrstr_free(mrstr_ptr_t block)
{
    free(block);
}

#else
#define mrstr_alloc(s) (malloc((size_t)(s)))
#define mrstr_realloc(b, s) (realloc((b), (size_t)(s)))
#define mrstr_free free
#endif

#endif /* __MRSTR_ALLOC__ */

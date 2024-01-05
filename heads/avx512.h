/*
    MRFStr Library version 0.1.0
    MetaReal Fast String Library
*/

#ifndef __MR_AVX512__
#define __MR_AVX512__

#ifdef _MSC_VER
#include <simddef.h>
#endif

#ifdef __AVX512F__
#include <immintrin.h>
#include <mrfstr.h>

#define MRFSTR_AVX512_TEQUAL_LOAD 0x10000
#define MRFSTR_AVX512_TCONTAIN_LOAD 0x10000

void __mrfstr_avx512_copy(
    mrfstr_ptr_t dst, mrfstr_ptr_ct src, mrfstr_size_t size);
void __mrfstr_avx512_fill(
    mrfstr_ptr_t res, mrfstr_chr_t chr, mrfstr_size_t size);

mrfstr_bool_t __mrfstr_avx512_equal(
    mrfstr_ptr_ct str1, mrfstr_ptr_ct str2, mrfstr_size_t size);
void __mrfstr_avx512_tequal(
    volatile mrfstr_bool_t *res,
    mrfstr_ptr_ct str1, mrfstr_ptr_ct str2, mrfstr_size_t size);

mrfstr_bool_t __mrfstr_avx512_contain(
    mrfstr_ptr_ct str, mrfstr_chr_t chr, mrfstr_size_t size);
void __mrfstr_avx512_tcontain(
    volatile mrfstr_bool_t *res,
    mrfstr_ptr_ct str, mrfstr_chr_t chr, mrfstr_size_t size);

#endif

#endif

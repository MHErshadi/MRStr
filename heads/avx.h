/*
    MRFStr Library version 0.1.0
    MetaReal Fast String Library
*/

#ifndef __MR_AVX__
#define __MR_AVX__

#ifdef _MSC_VER
#include <simddef.h>
#endif

#ifdef __AVX__
#include <immintrin.h>
#include <mrfstr.h>

#define MRFSTR_AVX_TEQUAL_LOAD 0x20000
#define MRFSTR_AVX_TCONTCHR_LOAD 0x20000
#define MRFSTR_AVX_TFINDCHR_LOAD 0x20000

void __mrfstr_avx_copy(
    mrfstr_ptr_t dst, mrfstr_ptr_ct src, mrfstr_size_t size);
void __mrfstr_avx_fill(
    mrfstr_ptr_t res, mrfstr_chr_t chr, mrfstr_size_t size);

#ifdef __AVX2__
void __mrfstr_avx_rev(
    mrfstr_ptr_t left, mrfstr_ptr_t right, mrfstr_size_t size);
void __mrfstr_avx_rev2(
    mrfstr_ptr_t left, mrfstr_ptr_ct right, mrfstr_size_t size);

void __mrfstr_avx_replchr(
    mrfstr_ptr_t str,
    mrfstr_chr_t old, mrfstr_chr_t new,
    mrfstr_size_t size);
void __mrfstr_avx_replchr2(
    mrfstr_ptr_t res, mrfstr_ptr_ct str,
    mrfstr_chr_t old, mrfstr_chr_t new,
    mrfstr_size_t size);

mrfstr_bool_t __mrfstr_avx_equal(
    mrfstr_ptr_ct str1, mrfstr_ptr_ct str2, mrfstr_size_t size);
void __mrfstr_avx_tequal(
    volatile mrfstr_bool_t *res,
    mrfstr_ptr_ct str1, mrfstr_ptr_ct str2, mrfstr_size_t size);

mrfstr_bool_t __mrfstr_avx_contchr(
    mrfstr_ptr_ct str, mrfstr_chr_t chr, mrfstr_size_t size);
void __mrfstr_avx_tcontchr(
    volatile mrfstr_bool_t *res,
    mrfstr_ptr_ct str, mrfstr_chr_t chr, mrfstr_size_t size);

mrfstr_idx_t __mrfstr_avx_findchr(
    mrfstr_ptr_ct str, mrfstr_chr_t chr, mrfstr_size_t size);
mrfstr_idx_t __mrfstr_avx_tfindchr(
    volatile mrfstr_idx_t *res, mrfstr_idx_t start,
    mrfstr_ptr_ct str, mrfstr_chr_t chr, mrfstr_size_t size);

mrfstr_size_t __mrfstr_avx_strlen(
    mrfstr_ptr_ct str);
#endif

#endif
#endif

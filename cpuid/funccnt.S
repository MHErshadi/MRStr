# MIT License
#
# Copyright (c) 2023 MetaReal
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# mrfstr_ubyte_t mrfstr_cpuid_funccnt(
#     mrfstr_ubyte_t *ext)

#ifdef WIN32
#define ext1 %rcx
#define ext2 %r8
#else
#define ext1 %rsi
#define ext2 %rsi
#endif

.bss
    .globl _funccnt
    .globl _extcnt
_funccnt:
    .byte 0
_extcnt:
    .zero 1

.text
    .globl mrfstr_cpuid_funccnt
mrfstr_cpuid_funccnt:
    cmpb $0, _funccnt(%rip)
    jne SAVED

    push %rbx
#ifdef WIN32
    mov ext1, ext2
#endif

    mov $0x80000000, %eax
    cpuid

    test ext2, ext2
    jz _EXTCNT_
    mov %al, (ext2)         # extended function count

_EXTCNT_:
    mov %al, _extcnt(%rip)

    xor %eax, %eax
    cpuid

    mov %al, _funccnt(%rip) # function count

    pop %rbx
    ret

SAVED:
    test ext1, ext1
    jz _FUNCCNT_S

    mov _extcnt(%rip), %al
    mov %al, (ext1)

_FUNCCNT_S:
    mov _funccnt(%rip), %al
    ret

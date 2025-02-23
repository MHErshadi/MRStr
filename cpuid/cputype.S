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

# mrfstr_ubyte_t mrfstr_cpuid_cputype(
#     mrfstr_ubyte_t *family, mrfstr_ubyte_t *model)

#ifdef WIN32
#define family1 %rcx
#define model1 %rdx
#define family2 %r10
#define model2 %r11
#else
#define family1 %rsi
#define model1 %rdi
#define family2 %rsi
#define model2 %rdi
#endif

.data
    .globl _is_intel
_is_intel:
    .byte 0xff

.bss
_vendor:
    .zero 1
_family:
    .zero 1
_model:
    .zero 1

.text
    .globl mrfstr_cpuid_cputype
mrfstr_cpuid_cputype:
    cmpb $0xff, _is_intel(%rip)
    jne SAVED

    push %rbx
#ifdef WIN32
    mov family1, family2
    mov model1, model2
#endif

    xor %eax, %eax
    cpuid

    cmp $0x6c65746e, %ecx   # ntel
    je VINTEL

    cmp $0x444d4163, %ecx   # cAMD
    je VAMD

    xor %r8b, %r8b          # unknown vendor
    movb $0, _is_intel(%rip)
    jmp FAMILY_MODEL

VINTEL:
    mov $1, %r8b            # Intel
    movb $1, _is_intel(%rip)
    jmp FAMILY_MODEL

VAMD:
    mov $2, %r8b            # AMD
    movb $0, _is_intel(%rip)
    jmp FAMILY_MODEL

FAMILY_MODEL:
    mov %r8b, _vendor(%rip)

    mov $1, %eax
    cpuid

    mov %eax, %ebx
    mov %eax, %r9d
    shr $8, %ebx
    and $15, %ebx
    shr $20, %r9d
    and $0xff, %r9d
    lea (%ebx, %r9d), %ebx  # family id

    test model2, model2
    jz _FAMILY_
    mov %bl, (model2)

_FAMILY_:
    mov %bl, _family(%rip)

    mov %al, %bl
    shr $4, %bl
    shr $12, %eax
    and $0xf0, %al
    or %bl, %al             # model

    test family2, family2
    jz _MODEL_
    mov %al, (family2)

_MODEL_:
    mov %al, _model(%rip)

    mov %r8b, %al
    pop %rbx
    ret

SAVED:
    test family1, family1
    jz _MODEL_S

    mov _family(%rip), %al
    mov %al, (family1)

_MODEL_S:
    test model1, model1
    jz _VENDOR_S

    mov _model(%rip), %al
    mov %al, (model1)

_VENDOR_S:
    mov _vendor(%rip), %al
    ret

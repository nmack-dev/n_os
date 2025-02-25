#pragma once

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

#define TRUE 1
#define FALSE 0
#define NULL ( (void*)0 )
#define PAGE_SIZE 4096

#define align_up( value, align )   __builtin_align_up( value, align )
#define is_aligned( value, align ) __builtin_is_aligned( value, aligned )
#define offsetof( type, member )   __builtin_offset( type, member )

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

void* memset
    (
    void* buf,
    char c,
    size_t n
    );

void* memcpy
    (
    void* dst,
    const void* src,
    size_t n
    );

char* strncpy
    (
    char* dst,
    const char* src,
    size_t n
    );

int strncmp
    (
    const char* s1,
    const char* s2,
    size_t n
    );

void printf
    (
    const char *fmt,
    ...
    );
#include "kernel.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t; // not supper portable lol

#define FOR_EVER for(;;)

extern char __bss[], __bss_end[], __stack_top[];

sbiret sbi_call
    ( 
    long arg0,
    long arg1,
    long arg2,
    long arg3,
    long arg4,
    long arg5,
    long fid,
    long eid
    )
{
register long a0 __asm__( "a0" ) = arg0;
register long a1 __asm__( "a1" ) = arg1;
register long a2 __asm__( "a2" ) = arg2;
register long a3 __asm__( "a3" ) = arg3;
register long a4 __asm__( "a4" ) = arg4;
register long a5 __asm__( "a5" ) = arg5;
register long a6 __asm__( "a6" ) = fid;
register long a7 __asm__( "a7" ) = eid;

__asm__ __volatile__
    (
    "ecall"
    : "=r"(a0), "=r"(a1)
    : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
      "r"(a6), "r"(a7)
    : "memory"
    );

    return( (sbiret){ .error = a0, .value = a1 } );
}

void putchar
    (
    char ch
    )
{
// Console Putchar
sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 );
}

void* memset
    ( 
    void* buf, 
    char c, 
    size_t n 
    )
{
uint8_t* p = (uint8_t *)buf;

while( n-- > 0 )
    {
    *p++ = c;
    }

return( buf );
}

void kernel_main
    (
     void 
    )
{
memset(__bss, 0, (size_t)__bss_end - (size_t)__bss );

const char* s = "\n\nHello World!\n";
for( int i = 0; s[ i ] != '\0'; ++i )
    {
    putchar( s[ i ] );
    }

FOR_EVER
    {
    __asm__ __volatile__("wfi");
    }
}

__attribute__((section(".text.boot")))
__attribute__((naked))  // tells clang NOT to do any compiler magic after
void boot
    ( 
    void 
    ) 
{
__asm__ __volatile__
    (
    "mv sp, %[stack_top]\n" // set the stack pointer
    "j kernel_main\n"       // jump to the kernel main function
    :
    : [stack_top] "r" (__stack_top) // pass the stack top address as %[stack_top]
    );
}
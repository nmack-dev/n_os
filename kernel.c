/*
Includes
*/
#include "kernel.h"
#include "common.h"
#include "assert.h"

/*
Defs
*/
#define COMPILE_TEST_CODE 0

/*
Macros
*/
#define FOR_EVER for(;;)

#define TO_PAGES( n ) n * PAGE_SIZE

/*
Externs
*/
extern char __bss[], __bss_end[], __stack_top[];
extern char __free_ram[], __free_ram_end[];

/*
Private Globals
*/
static process s_procs[ PROCS_MAX ];

/*
Functions
*/
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

__attribute__( ( naked ) )
__attribute__( ( aligned( 4 ) ) )
void kernel_entry
    (
    void
    )
{
__asm__ __volatile__
    (
    "csrw sscratch, sp\n"
    "addi sp, sp, -4 * 31\n"
    "sw ra,  4 * 0(sp)\n"
    "sw gp,  4 * 1(sp)\n"
    "sw tp,  4 * 2(sp)\n"
    "sw t0,  4 * 3(sp)\n"
    "sw t1,  4 * 4(sp)\n"
    "sw t2,  4 * 5(sp)\n"
    "sw t3,  4 * 6(sp)\n"
    "sw t4,  4 * 7(sp)\n"
    "sw t5,  4 * 8(sp)\n"
    "sw t6,  4 * 9(sp)\n"
    "sw a0,  4 * 10(sp)\n"
    "sw a1,  4 * 11(sp)\n"
    "sw a2,  4 * 12(sp)\n"
    "sw a3,  4 * 13(sp)\n"
    "sw a4,  4 * 14(sp)\n"
    "sw a5,  4 * 15(sp)\n"
    "sw a6,  4 * 16(sp)\n"
    "sw a7,  4 * 17(sp)\n"
    "sw s0,  4 * 18(sp)\n"
    "sw s1,  4 * 19(sp)\n"
    "sw s2,  4 * 20(sp)\n"
    "sw s3,  4 * 21(sp)\n"
    "sw s4,  4 * 22(sp)\n"
    "sw s5,  4 * 23(sp)\n"
    "sw s6,  4 * 24(sp)\n"
    "sw s7,  4 * 25(sp)\n"
    "sw s8,  4 * 26(sp)\n"
    "sw s9,  4 * 27(sp)\n"
    "sw s10, 4 * 28(sp)\n"
    "sw s11, 4 * 29(sp)\n"

    "csrr a0, sscratch\n"
    "sw a0, 4 * 30(sp)\n"

    "mv a0, sp\n"
    "call handle_trap\n"

    "lw ra,  4 * 0(sp)\n"
    "lw gp,  4 * 1(sp)\n"
    "lw tp,  4 * 2(sp)\n"
    "lw t0,  4 * 3(sp)\n"
    "lw t1,  4 * 4(sp)\n"
    "lw t2,  4 * 5(sp)\n"
    "lw t3,  4 * 6(sp)\n"
    "lw t4,  4 * 7(sp)\n"
    "lw t5,  4 * 8(sp)\n"
    "lw t6,  4 * 9(sp)\n"
    "lw a0,  4 * 10(sp)\n"
    "lw a1,  4 * 11(sp)\n"
    "lw a2,  4 * 12(sp)\n"
    "lw a3,  4 * 13(sp)\n"
    "lw a4,  4 * 14(sp)\n"
    "lw a5,  4 * 15(sp)\n"
    "lw a6,  4 * 16(sp)\n"
    "lw a7,  4 * 17(sp)\n"
    "lw s0,  4 * 18(sp)\n"
    "lw s1,  4 * 19(sp)\n"
    "lw s2,  4 * 20(sp)\n"
    "lw s3,  4 * 21(sp)\n"
    "lw s4,  4 * 22(sp)\n"
    "lw s5,  4 * 23(sp)\n"
    "lw s6,  4 * 24(sp)\n"
    "lw s7,  4 * 25(sp)\n"
    "lw s8,  4 * 26(sp)\n"
    "lw s9,  4 * 27(sp)\n"
    "lw s10, 4 * 28(sp)\n"
    "lw s11, 4 * 29(sp)\n"
    "lw sp,  4 * 30(sp)\n"
    "sret\n"
    );
}

void handle_trap
    (
    trap_frame* f
    )
{
assert_ptr( f );

uint32_t scause  = READ_CSR( scause );
uint32_t stval   = READ_CSR( stval );
uint32_t user_pc = READ_CSR( sepc );

panic( "unexpected trap scause=%x, stval=%x, sepc=%x", scause, stval, user_pc );
}

paddr_t alloc_pages
    (
    uint32_t n
    )
{
// TODO probably should implement free at some point
// would probably want to globalize next_paddr and add concurrency safety??

static paddr_t next_paddr = (paddr_t)__free_ram;
paddr_t paddr = next_paddr;
next_paddr += TO_PAGES( n );

if( next_paddr > (paddr_t)__free_ram_end )
    {
    panic( "Out of memory" );
    }

memset( (void*)paddr, 0, TO_PAGES( n ) );

return( paddr );
}

__attribute__( ( naked ) ) void switch_context
    (
    uint32_t* prev_sp,
    uint32_t* next_sp
    )
{
__asm__ __volatile__
    (
    /* save callee-saved register onto the current processe's stack */
    "addi sp, sp, -13 * 4\n" /* allocate stack space for 13 4 byte registers */
    "sw ra,  0  * 4(sp)\n"   /* save callee-saved registers only */
    "sw s0,  1  * 4(sp)\n"
    "sw s1,  2  * 4(sp)\n"
    "sw s2,  3  * 4(sp)\n"
    "sw s3,  4  * 4(sp)\n"
    "sw s4,  5  * 4(sp)\n"
    "sw s5,  6  * 4(sp)\n"
    "sw s6,  7  * 4(sp)\n"
    "sw s7,  8  * 4(sp)\n"
    "sw s8,  9  * 4(sp)\n"
    "sw s9,  10 * 4(sp)\n"
    "sw s10, 11 * 4(sp)\n"
    "sw s11, 12 * 4(sp)\n"

    /* switch stack pointer */
    "sw sp, (a0)\n"         /* *prev_sp = sp */
    "lw sp, (a1)\n"         /* switch stack pointer <--- here */

    /* restore callee saved registers from the next process's stack */
    "lw ra,  0  * 4(sp)\n"  /* restore callee-saved registers only */
    "lw s0,  1  * 4(sp)\n"
    "lw s1,  2  * 4(sp)\n"
    "lw s2,  3  * 4(sp)\n"
    "lw s3,  4  * 4(sp)\n"
    "lw s4,  5  * 4(sp)\n"
    "lw s5,  6  * 4(sp)\n"
    "lw s6,  7  * 4(sp)\n"
    "lw s7,  8  * 4(sp)\n"
    "lw s8,  9  * 4(sp)\n"
    "lw s9,  10 * 4(sp)\n"
    "lw s10, 11 * 4(sp)\n"
    "lw s11, 12 * 4(sp)\n"

    "addi sp, sp, 13 * 4\n" /* pop the 13 4 byte registers from the sack*/
    "ret\n"
    );
}

process* create_process
    (
    uint32_t pc
    )
{
process* proc = NULL;
uint32_t* sp = NULL;
size_t i;

for( i = 0; i < PROCS_MAX; ++i )
    {
    if( PROC_UNUSED == s_procs[ i ].state )
        {
        proc = &s_procs[ i ];
        break;
        }
    }

// TODO might wanna handle more gracefully
assert_w_msg( ( NULL != proc ), "No free process slots!" );

sp = (uint32_t*)&proc->stack[ sizeof( proc->stack ) ];
*--sp = 0;              /* s11 */
*--sp = 0;              /* s10 */
*--sp = 0;              /* s9  */
*--sp = 0;              /* s8  */
*--sp = 0;              /* s7  */
*--sp = 0;              /* s6  */
*--sp = 0;              /* s5  */
*--sp = 0;              /* s4  */
*--sp = 0;              /* s3  */
*--sp = 0;              /* s2  */
*--sp = 0;              /* s1  */
*--sp = 0;              /* s0  */
*--sp = (uint32_t)sp;   /* ra  */

proc->pid = i + 1;
proc->state = PROC_RUNNABLE;
proc->sp = (uint32_t)sp;

return( proc );
}

void delay
    (
    void
    )
{
// TODO allow delay input
for( int i = 0; i < 30000000; ++i )
    {
    __asm__ __volatile__( "nop" );
    }
}

/*
TEST - TODO REMOVE ME
*/
#if( COMPILE_TEST_CODE )
process* proc_a;
process* proc_b;

void proc_a_entry
    (
    void
    )
{
printf( "starting proc a" );

FOR_EVER
    {
    putchar('A');
    switch_context( &proc_a->sp, &proc_b->sp );
    delay();
    }
}

void proc_b_entry
    (
    void
    )
{
printf( "starting proc b" );

FOR_EVER
    {
    putchar('B');
    switch_context( &proc_b->sp, &proc_a->sp );
    delay();
    }
}
#endif
/*
END TEST - TODO REMOVE ME
*/

/*
Main & Boot - should be last
*/
void kernel_main
    (
     void
    )
{
memset(__bss, 0, (size_t)__bss_end - (size_t)__bss );

// WRITE_CSR( stvec, (uint32_t)kernel_entry );

#if( COMPILE_TEST_CODE )
proc_a = create_process( (uint32_t)proc_a_entry );
proc_b = create_process( (uint32_t)proc_b_entry );
proc_a_entry();
#endif

FOR_EVER
    {
    __asm__ __volatile__("wfi");
    }
}

__attribute__( (section( ".text.boot" ) ) )
__attribute__( ( naked ) )  // tells clang NOT to do any compiler magic after
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
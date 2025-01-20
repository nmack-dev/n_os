#include "common.h"
#include "assert.h"

void putchar(char ch);

void* memset
    (
    void* buf,
    char c,
    size_t n
    )
{
assert_ptr( buf );

uint8_t* bp = (uint8_t*)buf;

for( size_t i = 0; i < n; ++i )
    {
    *bp = c;
    ++bp;
    }

assert_ptr( buf );
return( buf );
}

void* memcpy
    (
    void* dst,
    const void* src,
    size_t n
    )
{
assert_ptr( dst );
assert_ptr( src );

uint8_t* d = (uint8_t*)dst;
const uint8_t* s = (const uint8_t*)src;

for( size_t i = 0; i < n; ++i )
    {
    *d = *s;
    ++d;
    ++s;
    }

assert_ptr( dst );
return( dst );
}

char* strncpy
    (
    char* dst,
    const char* src,
    size_t n
    )
{
assert_ptr( dst );
assert_ptr( src );

for( size_t i = 0; i < n; ++i )
    {
    *dst = *src;
    ++dst;
    ++src;
    }

*dst = '\0';
return( dst );
}

int strcmp
    (
    const char* s1,
    const char* s2,
    size_t n
    )
{
for( size_t i = 0; i < n; ++i )
    {
    if( *s1 != *s2 )
        {
        break;
        }

    ++s1;
    ++s2;
    }

    return( *(unsigned char*)s1 - *(unsigned char*)s2 );
}

void printf
    (
    const char* fmt,
    ...
    )
{
assert( NULL != fmt );

va_list vargs;
va_start( vargs, fmt );

while( *fmt )
    {
    if( '%' == *fmt )
        {
        ++fmt;

        switch( *fmt )  // read next
            {
            case '\0':  // % char at the end
                {
                putchar('%');
                goto end;
                }

            case '%':   // print %
                {
                putchar( '%' );
                break;
                }

            case 's':   // print null terminated string
                {
                const char* s = va_arg( vargs, const char* );
                while( *s )
                    {
                    putchar( *s );
                    ++s;
                    }

                break;
                }

            case 'd':   // print signed integer
                {
                int value = va_arg( vargs, int );
                int divisor = 1;

                if( value < 0 )
                    {
                    putchar('-');
                    value = -value;
                    }

                while( value / divisor > 9 )
                    {
                    divisor *= 10;
                    }

                while( divisor > 0 )
                    {
                    putchar( '0' + value / divisor );
                    value %= divisor;
                    divisor /= 10;
                    }

                break;
                }

            case 'x':   // print hex
                {
                int nibble;

                int value = va_arg( vargs, int );

                for( int i = 7; i >= 0; --i )
                    {
                    nibble = ( value >> ( i * 4 ) ) & 0xf;
                    putchar( "0123456789abcdef"[ nibble ] );
                    }
                }
            }
        }
        else
        {
        putchar( *fmt );
        }

        ++fmt;
    }

end:
    va_end( vargs );
}
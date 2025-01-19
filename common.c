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
assert( NULL != buf );

uint8_t* bp = (uint8_t*)buf;

for( size_t i = 0; i < n; ++i )
    {
    *bp = c;
    ++bp;
    }

return( buf );
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
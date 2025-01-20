#pragma once

#include "common.h"

#define assert_w_msg( condition, fmt, ... )\
    do\
    {\
    if( !( condition ) )\
        {\
        printf("ASSERT: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__ );\
        while( 1 ){}\
        }\
    }\
    while( 0 )

#define assert( condition )\
    assert_w_msg( condition, "generic error - shutting down kernel" );

#define assert_ptr( ptr )\
    assert_w_msg( ( NULL != ptr ), "null pointer" )

#define panic( fmt, ... )\
    assert_w_msg( 0, fmt, __VA_ARGS__ )
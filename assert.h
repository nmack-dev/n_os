#pragma once

#include "common.h"

#define panic( fmt, ... )\
    do\
    {\
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__ );\
    while( 1 ){}\
    }\
    while( 0 )

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
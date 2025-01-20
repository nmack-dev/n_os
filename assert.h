#define assert( condition )\
    do\
    {\
    if( !( condition ) )\
        {\
        __builtin_trap();\
        }\
    }\
    while( 0 )

#define assert_ptr( ptr )\
    assert( NULL != ptr )
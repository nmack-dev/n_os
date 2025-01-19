#define assert( condition )\
    do\
    {\
    if( !( condition ) )\
        {\
        __builtin_trap();\
        }\
    }\
    while( 0 )
# n_os
Another attempt at building a kernel via tutorials
clone & run `./run.sh` in WSL

## 01/19/25
- Boots via OpenSBI and outputs `Hello World!` to qemu terminal
- printf implementation and basic asserting
- better memset impl + more utl function defs
- clang is awesome

## 01/20/25
- memset, strncmp, strncpy
- better asserting
- trap handler for kernel level errors - e.g. cpu black magic

## 01/25/25
- add a page allocator, still need free functionality
- add basic process support and ability to context switch
    - surprisingly pretty easy to do
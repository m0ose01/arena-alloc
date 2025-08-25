# Arena Allocator

A dead-simple arena allocator written in C11.

This library provides four functions:
`arena_new` to generate a new arena with a specified capacity, `arena_alloc` to obtain memory from the arena, `arena_reset` to reset the arena (without clearing or freeing any memory) and `arena_free` to destroy the memory.

This repository contains a CMakeLists.txt file which will generate a static library, `arena`, which you can link to (as well as a header file `arena.h`).

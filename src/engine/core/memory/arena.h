#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include "engine/core/define.h" // IWYU pragma: keep

typedef struct {
    u64 total_size;
    u64 prev_offset;
    u64 curr_offset;
    void *memory;
    b8 own_memory;
} arena_alloc_t;

b8 arena_create(u64 total_size, arena_alloc_t *arena, void *memory);

void arena_kill(arena_alloc_t *arena);

void *arena_alloc_align(arena_alloc_t *arena, u64 size, u8 alignment);

void *arena_alloc(arena_alloc_t *arena, u64 size);

void arena_reset(arena_alloc_t *arena);

u64 arena_remaining(const arena_alloc_t *arena);

u64 arena_used(const arena_alloc_t *arena);

#endif // ARENA_ALLOC_H

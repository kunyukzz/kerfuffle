#ifndef MEMORY_H
#define MEMORY_H

#include "engine/core/define.h" // IWYU pragma: keep

typedef enum {
    MEM_UNKNOWN = 0x00,
    MEM_GAME,
    MEM_ARENA,
    MEM_RENDER,
    MEM_AUDIO,
    MEM_ARRAY,
    MEM_DYNARRAY,
    MEM_RESOURCE,
    MEM_MAX_TAG
} memtag_t;

#define ALLOC(size, tag) alloc_dbg(size, tag, __FILE__, __LINE__)

#define FREE(block, size, tag) alloc_free(block, size, tag)

b8 memory_sys_init(u64 total_size);

void memory_sys_kill(void);

void *alloc_dbg(u64 size, memtag_t tag, const char *file, u32 line);

void alloc_free(void *block, u64 size, memtag_t tag);

char *mem_debug_stat(void);

#endif // MEMORY_H

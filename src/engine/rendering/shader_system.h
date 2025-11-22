#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/memory/arena.h"

typedef struct {
    arena_alloc_t *arena;
    u32 program;
} shader_system_t;

shader_system_t *shader_sys_init(arena_alloc_t *arena);

void shader_sys_kill(shader_system_t *sh);

b8 shader_sys_set(shader_system_t *sh, const char *name);

void shader_sys_bind(shader_system_t *sh);

#endif // SHADER_SYSTEM_H

#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/memory/arena.h"
#include "engine/core/math/math_types.h"

typedef struct {
    u32 program;
    i32 model;
    i32 view;
    i32 proj;
} shader_t;

typedef struct {
    arena_alloc_t *arena;
    shader_t object_shader;
} shader_system_t;

shader_system_t *shader_sys_init(arena_alloc_t *arena);

void shader_sys_kill(shader_system_t *sh);

b8 shader_sys_set(shader_system_t *sh, const char *name);

void shader_sys_bind(shader_system_t *sh);

void shader_sys_set_uniform_mat4(shader_system_t *sh, mat4 matrix);

#endif // SHADER_SYSTEM_H

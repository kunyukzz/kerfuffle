#include "shader_system.h"
#include "render.h"
#include "deps/glad/glad.h"

// std
#include <string.h>

shader_system_t *shader_sys_init(arena_alloc_t *arena)
{
    shader_system_t *sh = arena_alloc(arena, sizeof(shader_system_t));
    if (!sh) return NULL;
    memset(sh, 0, sizeof(shader_system_t));

    sh->arena = arena;
    sh->program = 0;

    LOG_INFO("Shader System Init");
    return sh;
}

void shader_sys_kill(shader_system_t *sh)
{
    if (!sh) return;

    memset(sh, 0, sizeof(shader_system_t));
    LOG_INFO("Shader System Kill");
}

b8 shader_sys_set(shader_system_t *sh, const char *name)
{
    u32 program = render_upload_shader(name);
    sh->program = program;
    return true;
}

void shader_sys_bind(shader_system_t *sh)
{
    if (sh->program) glUseProgram(sh->program);
}

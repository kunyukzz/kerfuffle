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
    glDeleteProgram(sh->program);

    memset(sh, 0, sizeof(shader_system_t));
    LOG_INFO("Shader System Kill");
}

b8 shader_sys_set(shader_system_t *sh, const char *name)
{
    u32 program = render_upload_shader(name);
    sh->program = program;

    GLuint block = glGetUniformBlockIndex(sh->program, "camera_block");
    if (block == GL_INVALID_INDEX)
        LOG_WARN("invalid dumbass!!!");
    else
        glUniformBlockBinding(sh->program, block, 0);

    sh->model = glGetUniformLocation(sh->program, "model");
    if (sh->model == -1) LOG_WARN("uniform model not found");
    return true;
}

void shader_sys_bind(shader_system_t *sh)
{
    if (sh->program) glUseProgram(sh->program);
}

void shader_sys_set_uniform_mat4(shader_system_t *sh, mat4 matrix)
{
    glUniformMatrix4fv(sh->model, 1, GL_FALSE, matrix.data);
}

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
    sh->object_shader.program = 0;

    LOG_INFO("Shader System Init");
    return sh;
}

void shader_sys_kill(shader_system_t *sh)
{
    if (!sh) return;
    glDeleteProgram(sh->object_shader.program);

    memset(sh, 0, sizeof(shader_system_t));
    LOG_INFO("Shader System Kill");
}

b8 shader_sys_set(shader_system_t *sh, const char *name)
{
    u32 program = render_upload_shader(name);
    sh->object_shader.program = program;

    GLuint block =
        glGetUniformBlockIndex(sh->object_shader.program, "camera_block");
    if (block == GL_INVALID_INDEX)
        LOG_WARN("invalid dumbass!!!");
    else
        glUniformBlockBinding(sh->object_shader.program, block, 0);

    // NOTE: this for caching uniform
    sh->object_shader.model =
        glGetUniformLocation(sh->object_shader.program, "model");
    if (sh->object_shader.model == -1) LOG_WARN("uniform model not found");
    return true;
}

void shader_sys_bind(shader_system_t *sh)
{
    if (sh->object_shader.program) glUseProgram(sh->object_shader.program);
}

void shader_sys_set_uniform_mat4(shader_system_t *sh, mat4 matrix)
{
    glUniformMatrix4fv(sh->object_shader.model, 1, GL_FALSE, matrix.data);
}

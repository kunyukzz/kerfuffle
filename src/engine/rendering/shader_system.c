#include "shader_system.h"
#include "render.h"
#include "deps/glad/glad.h"

// std
#include <string.h>

static shader_system_t *g_sh = NULL;

shader_system_t *shader_sys_init(arena_alloc_t *arena)
{
    shader_system_t *sh = arena_alloc(arena, sizeof(shader_system_t));
    if (!sh) return NULL;
    memset(sh, 0, sizeof(shader_system_t));

    sh->arena = arena;

    sh->object_shader.program = 0;
    // sh->object_shader.model = -1;
    // sh->object_shader.light_pos = -1;
    // sh->object_shader.view_pos = -1;
    // sh->object_shader.light_color = -1;
    // sh->object_shader.object_color = -1;

    // Initialize light shader
    sh->light_shader.program = 0;
    // sh->light_shader.model = -1;
    // sh->light_shader.light_pos = -1;
    // sh->light_shader.view_pos = -1;
    // sh->light_shader.light_color = -1;
    // sh->light_shader.object_color = -1;

    g_sh = sh;
    LOG_INFO("Shader System Init");
    return sh;
}

void shader_sys_kill(shader_system_t *sh)
{
    if (!g_sh) return;
    glDeleteProgram(sh->object_shader.program);
    glDeleteProgram(sh->light_shader.program);

    memset(sh, 0, sizeof(shader_system_t));
    LOG_INFO("Shader System Kill");
}

b8 shader_sys_set(shader_t *shader, const char *name)
{
    u32 program = render_upload_shader(name);
    shader->program = program;

    GLuint block = glGetUniformBlockIndex(shader->program, "camera_block");
    if (block == GL_INVALID_INDEX)
        LOG_WARN("invalid dumbass!!!");
    else
        glUniformBlockBinding(shader->program, block, 0);

    // NOTE: this for caching uniform
    shader->model = glGetUniformLocation(shader->program, "model");

    shader->light_pos = glGetUniformLocation(shader->program, "light_pos");
    shader->view_pos = glGetUniformLocation(shader->program, "view_pos");
    shader->light_color = glGetUniformLocation(shader->program, "light_color");
    shader->object_color =
        glGetUniformLocation(shader->program, "object_color");
    return true;
}

void shader_sys_bind(shader_t *shader)
{
    if (shader->program) glUseProgram(shader->program);
}

void shader_sys_set_uniform_mat4(shader_t *shader, mat4 matrix)
{
    if (shader->model == -1) return;
    glUniformMatrix4fv(shader->model, 1, GL_FALSE, matrix.data);
}

void shader_sys_set_vec3(shader_t *shader, i32 loc, vec3 v)
{
    (void)shader;
    if (loc == -1) return;
    glUniform3fv(loc, 1, &v.x);
}

#include "render.h"
#include "engine/core/math/math_types.h"
#include "engine/resource/resc_loader.h"

#include "deps/glad/glad.h"

// std
#include <string.h>
#include <stdio.h>

static GLuint compile_shader(GLenum type, const char *src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        LOG_ERROR("Shader compile error: %s", info);
    }
    return shader;
}

static void init_mesh(render_system_t *rs)
{
    glGenVertexArrays(1, &rs->rs_mesh->vao);
    glBindVertexArray(rs->rs_mesh->vao);

    glGenBuffers(1, &rs->rs_mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rs->rs_mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, rs->vert_count * sizeof(f32), rs->vert,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &rs->rs_mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rs->rs_mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rs->indcs_count * sizeof(u32),
                 rs->indcs, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32),
                          (void *)0);

    glBindVertexArray(0);
}

render_system_t *render_sys_init(arena_alloc_t *arena)
{
    render_system_t *rs = arena_alloc(arena, sizeof(render_system_t));
    if (!rs) return NULL;
    memset(rs, 0, sizeof(render_system_t));

    rs->arena = arena;
    rs->rs_mesh = ALLOC(sizeof(render_mesh_t), MEM_RENDER);

    // glad setup
    int version_glad = gladLoadGL();
    if (version_glad == 0)
    {
        LOG_FATAL("Failed to initialize OpenGL context");
        return NULL;
    }

    rs->current_fbo = 0;
    rs->clear_color = (vec4){{1.0f, 0.0f, 0.0f, 1.0f}};

    // set renderpass
    rs->main_pass = (render_pass_t){.fbo = rs->main_fbo,
                                    .clear_color = {{1.0f, 0.0f, 0.0f, 1.0f}},
                                    .clear_mask = GL_COLOR_BUFFER_BIT |
                                                  GL_DEPTH_BUFFER_BIT};

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);

    f32 vert[] = {
        -0.5f, 0.5f,  0.0f, // tl
        0.5f,  0.5f,  0.0f, // tr
        -0.5f, -0.5f, 0.0f, // bl
        0.5f,  -0.5f, 0.0f, // br
    };

    unsigned int indcs[] = {
        0, 2, 3, // first
        0, 3, 1  // second
    };

    rs->vert = vert;
    rs->vert_count = 12;
    rs->indcs = indcs;
    rs->indcs_count = 6;

    init_mesh(rs);
    // glUseProgram(rs->main_pass.shader_program);

    LOG_TRACE("Renderer: %s", glGetString(GL_RENDERER));
    LOG_INFO("Render System Init");
    return rs;
}

void render_sys_kill(render_system_t *rs)
{
    if (!rs) return;

    FREE(rs->rs_mesh, sizeof(render_mesh_t), MEM_RENDER);
    memset(rs, 0, sizeof(render_system_t));
    LOG_INFO("Render System Kill");
}

void render_sys_begin(render_system_t *rs, u8 id)
{
    render_pass_t *pass = 0;

    switch (id)
    {
    case WORLD_PASS: pass = &rs->main_pass; break;
    case DEBUG_UI_PASS: break;
    }

    if (rs->current_fbo != pass->fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pass->fbo);
        rs->current_fbo = pass->fbo;
    }

    glClear(pass->clear_mask);
}

void render_sys_end(render_system_t *rs, u8 id)
{
    (void)rs;
    (void)id;
}

void render_draw(render_system_t *rs)
{
    glBindVertexArray(rs->rs_mesh->vao);
    glDrawElements(GL_TRIANGLES, (int)rs->indcs_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

u32 render_upload_shader(const char *name)
{
    char vert_path[MAX_PATH];
    char frag_path[MAX_PATH];
    snprintf(vert_path, sizeof(vert_path), "%s.vert.glsl", name);
    snprintf(frag_path, sizeof(frag_path), "%s.frag.glsl", name);

    u64 vert_size = 0, frag_size = 0;
    void *vert_src = read_file_text(vert_path, &vert_size);
    void *frag_src = read_file_text(frag_path, &frag_size);

    if (!vert_src || !frag_src)
    {
        LOG_ERROR("Failed to load shader files: %s", name);
        if (vert_src) FREE(vert_src, vert_size, MEM_RESOURCE);
        if (frag_src) FREE(frag_src, frag_size, MEM_RESOURCE);
        return 0;
    }

    GLuint vert = compile_shader(GL_VERTEX_SHADER, vert_src);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);

    if (!vert || !frag)
    {
        LOG_ERROR("Failed to compile shaders: %s", name);
        FREE(vert_src, vert_size, MEM_RESOURCE);
        FREE(frag_src, frag_size, MEM_RESOURCE);

        if (vert) glDeleteShader(vert);
        if (frag) glDeleteShader(frag);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // Check link status
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        LOG_ERROR("Program link error: %s", info);

        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);
        FREE(vert_src, vert_size, MEM_RESOURCE);
        FREE(frag_src, frag_size, MEM_RESOURCE);
        return 0;
    }

    FREE(vert_src, vert_size, MEM_RESOURCE);
    FREE(frag_src, frag_size, MEM_RESOURCE);
    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

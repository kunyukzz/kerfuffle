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
    glBufferData(GL_ARRAY_BUFFER, rs->vertices_size, rs->vertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &rs->rs_mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rs->rs_mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rs->indices_size, rs->indices,
                 GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)OFFSETOF(vertex, position));

    glBindVertexArray(0);
}

render_system_t *render_sys_init(arena_alloc_t *arena)
{
    render_system_t *rs = arena_alloc(arena, sizeof(render_system_t));
    if (!rs) return NULL;
    memset(rs, 0, sizeof(render_system_t));

    rs->arena = arena;
    rs->cam = get_camera_system();
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
    glDepthFunc(GL_LESS);

    vertex *vert = ALLOC(sizeof(vertex) * 4, MEM_ARRAY);
    vert[0].position = (vec3){{-0.5f, 0.5f, 0.0f}};  // tl
    vert[1].position = (vec3){{0.5f, 0.5f, 0.0f}};   // tr
    vert[2].position = (vec3){{-0.5f, -0.5f, 0.0f}}; // bl
    vert[3].position = (vec3){{0.5f, -0.5f, 0.0f}};  // br

    u32 *indcs = ALLOC(sizeof(u32) * 6, MEM_ARRAY);
    // first
    indcs[0] = 0;
    indcs[1] = 2;
    indcs[2] = 3;

    // second
    indcs[3] = 0;
    indcs[4] = 3;
    indcs[5] = 1;

    rs->vertices = vert;
    rs->vertices_count = 4;
    rs->vertices_size = rs->vertices_count * sizeof(vertex);
    rs->indices = indcs;
    rs->indices_count = 6;
    rs->indices_size = rs->indices_count * sizeof(u32);

    init_mesh(rs);

    // world
    glGenBuffers(1, &rs->ubo_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, rs->ubo_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(render_ubo_t), NULL,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, rs->ubo_buffer);

    LOG_TRACE("Renderer: %s", glGetString(GL_RENDERER));
    LOG_INFO("Render System Init");
    return rs;
}

void render_sys_kill(render_system_t *rs)
{
    if (!rs) return;

    FREE(rs->vertices, sizeof(vertex) * rs->vertices_count, MEM_ARRAY);
    FREE(rs->indices, sizeof(u32) * rs->indices_count, MEM_ARRAY);

    glDeleteVertexArrays(1, &rs->rs_mesh->vao);
    glDeleteBuffers(1, &rs->rs_mesh->vbo);
    glDeleteBuffers(1, &rs->rs_mesh->ebo);

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

    /*
    printf("proj[0][0]=%f proj[1][1]=%f\n", rs->cam->world.proj.data[0],
           rs->cam->world.proj.data[5]);
    printf("view[0][0]=%f view[1][1]=%f\n", rs->cam->world.view.data[0],
           rs->cam->world.view.data[5]);
    */

    // world
    glBindBuffer(GL_UNIFORM_BUFFER, rs->ubo_buffer);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4),
                    rs->cam->world.proj.data);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4),
                    rs->cam->world.view.data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glClear(pass->clear_mask);
}

void render_sys_end(render_system_t *rs, u8 id)
{
    (void)rs;
    (void)id;
}

void render_sys_update(render_system_t *rs, f64 delta)
{
    (void)rs;
    (void)delta;
}

void render_draw(render_system_t *rs)
{
    glBindVertexArray(rs->rs_mesh->vao);
    glDrawElements(GL_TRIANGLES, (int)rs->indices_count, GL_UNSIGNED_INT, 0);
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

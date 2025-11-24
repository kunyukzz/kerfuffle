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

static void init_mesh(render_system_t *rs, render_mesh_t *mesh)
{
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, rs->vertices_size, rs->vertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // TODO: Temporary code start
    vertex *vert = ALLOC(sizeof(vertex) * 24, MEM_ARRAY);

    vec3 normal_front = (vec3){{0.0f, 0.0f, 1.0f}};
    vec3 normal_back = (vec3){{0.0f, 0.0f, -1.0f}};
    vec3 normal_right = (vec3){{1.0f, 0.0f, 0.0f}};
    vec3 normal_left = (vec3){{-1.0f, 0.0f, 0.0f}};
    vec3 normal_top = (vec3){{0.0f, 1.0f, 0.0f}};
    vec3 normal_bottom = (vec3){{0.0f, -1.0f, 0.0f}};

    vec2 uv_10 = (vec2){{1.0f, 0.0f}};
    vec2 uv_11 = (vec2){{1.0f, 1.0f}};
    vec2 uv_00 = (vec2){{0.0f, 0.0f}};
    vec2 uv_01 = (vec2){{0.0f, 1.0f}};

    // front
    vert[0] = (vertex){.position = (vec3){{-0.5f, 0.5f, 0.5f}},
                       .normal = normal_front,
                       .texcoord = uv_01};
    vert[1] = (vertex){.position = (vec3){{0.5f, 0.5f, 0.5f}},
                       .normal = normal_front,
                       .texcoord = uv_11};
    vert[2] = (vertex){.position = (vec3){{-0.5f, -0.5f, 0.5f}},
                       .normal = normal_front,
                       .texcoord = uv_00};
    vert[3] = (vertex){.position = (vec3){{0.5f, -0.5f, 0.5f}},
                       .normal = normal_front,
                       .texcoord = uv_10};
    // back
    vert[4] = (vertex){.position = (vec3){{-0.5f, 0.5f, -0.5f}},
                       .normal = normal_back,
                       .texcoord = uv_11};
    vert[5] = (vertex){.position = (vec3){{0.5f, 0.5f, -0.5f}},
                       .normal = normal_back,
                       .texcoord = uv_01};
    vert[6] = (vertex){.position = (vec3){{-0.5f, -0.5f, -0.5f}},
                       .normal = normal_back,
                       .texcoord = uv_10};
    vert[7] = (vertex){.position = (vec3){{0.5f, -0.5f, -0.5f}},
                       .normal = normal_back,
                       .texcoord = uv_00};
    // right
    vert[8] = (vertex){.position = (vec3){{0.5f, 0.5f, 0.5f}},
                       .normal = normal_right,
                       .texcoord = uv_01};
    vert[9] = (vertex){.position = (vec3){{0.5f, 0.5f, -0.5f}},
                       .normal = normal_right,
                       .texcoord = uv_11};
    vert[10] = (vertex){.position = (vec3){{0.5f, -0.5f, 0.5f}},
                        .normal = normal_right,
                        .texcoord = uv_00};
    vert[11] = (vertex){.position = (vec3){{0.5f, -0.5f, -0.5f}},
                        .normal = normal_right,
                        .texcoord = uv_10};
    // left
    vert[12] = (vertex){.position = (vec3){{-0.5f, 0.5f, 0.5f}},
                        .normal = normal_left,
                        .texcoord = uv_11};
    vert[13] = (vertex){.position = (vec3){{-0.5f, 0.5f, -0.5f}},
                        .normal = normal_left,
                        .texcoord = uv_01};
    vert[14] = (vertex){.position = (vec3){{-0.5f, -0.5f, 0.5f}},
                        .normal = normal_left,
                        .texcoord = uv_10};
    vert[15] = (vertex){.position = (vec3){{-0.5f, -0.5f, -0.5f}},
                        .normal = normal_left,
                        .texcoord = uv_00};
    // top
    vert[16] = (vertex){.position = (vec3){{-0.5f, 0.5f, -0.5f}},
                        .normal = normal_top,
                        .texcoord = uv_01};
    vert[17] = (vertex){.position = (vec3){{0.5f, 0.5f, -0.5f}},
                        .normal = normal_top,
                        .texcoord = uv_11};
    vert[18] = (vertex){.position = (vec3){{-0.5f, 0.5f, 0.5f}},
                        .normal = normal_top,
                        .texcoord = uv_00};
    vert[19] = (vertex){.position = (vec3){{0.5f, 0.5f, 0.5f}},
                        .normal = normal_top,
                        .texcoord = uv_10};
    // bottom
    vert[20] = (vertex){.position = (vec3){{-0.5f, -0.5f, -0.5f}},
                        .normal = normal_bottom,
                        .texcoord = uv_01};
    vert[21] = (vertex){.position = (vec3){{0.5f, -0.5f, -0.5f}},
                        .normal = normal_bottom,
                        .texcoord = uv_11};
    vert[22] = (vertex){.position = (vec3){{-0.5f, -0.5f, 0.5f}},
                        .normal = normal_bottom,
                        .texcoord = uv_00};
    vert[23] = (vertex){.position = (vec3){{0.5f, -0.5f, 0.5f}},
                        .normal = normal_bottom,
                        .texcoord = uv_10};

    // clang-format off
    u32 *indcs = ALLOC(sizeof(u32) * 36, MEM_ARRAY);
    // front
    indcs[0] = 0; indcs[1] = 2; indcs[2] = 3; // first
    indcs[3] = 0; indcs[4] = 3; indcs[5] = 1; // second
    // back
    indcs[6] = 4; indcs[7] = 7; indcs[8] = 6; // first
    indcs[9] = 4; indcs[10] = 5; indcs[11] = 7; // second
    // right 
    indcs[12] = 8; indcs[13] = 10; indcs[14] = 11; // first
    indcs[15] = 8; indcs[16] = 11; indcs[17] = 9; // second
    // left 
    indcs[18] = 12; indcs[19] = 15; indcs[20] = 14; // first
    indcs[21] = 12; indcs[22] = 13; indcs[23] = 15; // second
    // top
    indcs[24] = 16; indcs[25] = 18; indcs[26] = 19; // first
    indcs[27] = 16; indcs[28] = 19; indcs[29] = 17; // second
    // bottom
    indcs[30] = 20; indcs[31] = 23; indcs[32] = 22; // first
    indcs[33] = 20; indcs[34] = 21; indcs[35] = 23; // second
    // clang-format on

    rs->vertices = vert;
    rs->vertices_count = 24;
    rs->vertices_size = rs->vertices_count * sizeof(vertex);
    rs->indices = indcs;
    rs->indices_count = 36;
    rs->indices_size = rs->indices_count * sizeof(u32);

    init_mesh(rs, rs->rs_mesh);
    // TODO: Temporary code end

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

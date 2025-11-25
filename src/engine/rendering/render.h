#ifndef RENDERER_H
#define RENDERER_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/memory/arena.h"
#include "engine/rendering/camera_system.h"

typedef enum { WORLD_PASS = 0x01, DEBUG_UI_PASS = 0x02 } render_layer_t;

typedef struct {
    u32 fbo;
    vec4 clear_color;
    u32 clear_mask;
} render_pass_t;

typedef struct {
    u32 vao;
    u32 vbo;
    u32 ebo;
    u32 index_count;
} render_mesh_t;

typedef struct {
    mat4 proj;
    mat4 view;
} render_ubo_t;

typedef struct {
    void *vertices;
    u32 vert_count;
    u32 vert_size;
    void *indices;
    u32 indices_count;
    u32 indices_size;
} render_geo_t;

typedef struct {
    u32 vert_count;
    u32 vert_size;
    u32 vert_offset;

    u32 ind_count;
    u32 ind_size;
    u32 ind_offset;
} internal_geo_t;

typedef struct {
    arena_alloc_t *arena;
    camera_system_t *cam;

    render_pass_t main_pass;
    // render_pass_t test_pass;

    u32 current_fbo;
    vec4 clear_color;

    u32 main_fbo;
    // u32 test_fbo;

    render_mesh_t *rs_mesh;
    render_mesh_t *rs_quad;
    render_mesh_t *rs_light;

    u32 ubo_buffer;
    render_ubo_t ubo;

    // TODO: temp vertex data
    render_geo_t *geo;

} render_system_t;

render_system_t *render_sys_init(arena_alloc_t *arena);

void render_sys_kill(render_system_t *rs);

void render_sys_begin(render_system_t *rs, u8 id);

void render_sys_end(render_system_t *rs, u8 id);

void render_sys_update(render_system_t *rs, f64 delta);

void render_draw(render_system_t *rs);

void render_light(render_system_t *rs); // NOTE: temp code.

u32 render_upload_shader(const char *name);

#endif // RENDERER_H

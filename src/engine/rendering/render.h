#ifndef RENDERER_H
#define RENDERER_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/memory/arena.h"
#include "engine/core/math/math_types.h"

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
    arena_alloc_t *arena;
    render_pass_t main_pass;
    // render_pass_t test_pass;

    u32 current_fbo;
    vec4 clear_color;

    u32 main_fbo;
    u32 test_fbo;

    render_mesh_t *rs_mesh;
    f32 *vert;
    u32 vert_count;
    u32 *indcs;
    u32 indcs_count;
} render_system_t;

render_system_t *render_sys_init(arena_alloc_t *arena);

void render_sys_kill(render_system_t *rs);

void render_sys_begin(render_system_t *rs, u8 id);

void render_sys_end(render_system_t *rs, u8 id);

void render_draw(render_system_t *rs);

u32 render_upload_shader(const char *name);

#endif // RENDERER_H

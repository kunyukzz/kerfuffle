#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/memory/arena.h"
#include "engine/core/math/math_types.h"

typedef enum {
    CAMERA_PROJECTION_PERSPECTIVE,
    CAMERA_PROJECTION_ORTHOGRAPHIC
} camera_projection_type_t;

typedef struct {
    mat4 proj;
    mat4 view;

    f32 near, far, fov;
    f32 aspect_ratio;
    vec3 position, rotation;
    vec3 front, up, right;

    camera_projection_type_t proj_type;

    b8 dirty;
} camera_t;

typedef struct {
    arena_alloc_t *arena;
    camera_t world;
} camera_system_t;

camera_system_t *camera_sys_init(arena_alloc_t *arena);

void camera_sys_kill(camera_system_t *cs);

void camera_update(camera_system_t *cs);

camera_system_t *get_camera_system(void);

// Movement helper
void cam_yaw(camera_system_t *cam, f32 amount);

void cam_pitch(camera_system_t *cam, f32 amount);

void cam_move_forward(camera_system_t *cam, vec3 *velocity, f32 amount);

void cam_move_backward(camera_system_t *cam, vec3 *velocity, f32 amount);

void cam_move_left(camera_system_t *cam, vec3 *velocity, f32 amount);

void cam_move_right(camera_system_t *cam, vec3 *velocity, f32 amount);

void cam_move_up(camera_system_t *cam, vec3 *velocity, f32 amount);

void cam_move_down(camera_system_t *cam, vec3 *velocity, f32 amount);

void cam_apply_velocity(camera_system_t *cam, vec3 velo, f32 speed, f64 delta);

#endif // CAMERA_SYSTEM_H

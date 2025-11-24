#ifndef APPLICATION_H
#define APPLICATION_H

#include "define.h" // IWYU pragma: keep

#include "engine/core/clock.h"
#include "engine/core/memory/arena.h"
#include "engine/platform/filesystem.h"
#include "engine/platform/window.h"
#include "engine/platform/input.h"
#include "engine/rendering/camera_system.h"
#include "engine/rendering/render.h"
#include "engine/rendering/shader_system.h"

#include "game/game.h"

// #include "engine/core/math/math_test.h"

typedef struct {
    arena_alloc_t arena;
    clock_timer_t time;

    file_system_t *fs;
    window_system_t *ws;
    input_system_t *ip;

    camera_system_t *cs;
    render_system_t *rs;
    shader_system_t *sh;

    game_t *game;
} application_t;

b8 application_init(application_t *app);
b8 application_run(application_t *app);

#endif // APPLICATION_H

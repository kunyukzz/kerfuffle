#ifndef GAME_H
#define GAME_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/rendering/camera_system.h"

typedef struct {
    f64 delta;
    camera_system_t *cam;
} game_t;

game_t *game_init(void);

b8 game_update(game_t *game, f64 delta);

b8 game_render(game_t *game, f64 delta);

void game_kill(game_t *game);

#endif // GAME_H

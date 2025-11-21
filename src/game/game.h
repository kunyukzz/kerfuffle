#ifndef GAME_H
#define GAME_H

#include "engine/core/define.h" // IWYU pragma: keep

typedef struct {
    f64 delta;
} game_t;

game_t *game_init(void);

b8 game_update(game_t *game, f64 delta);

b8 game_render(game_t *game, f64 delta);

void game_kill(game_t *game);

#endif // GAME_H

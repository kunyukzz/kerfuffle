#include "game.h"
#include "engine/core/memory/memory.h"

game_t *game_init(void)
{
    game_t *game = ALLOC(sizeof(game_t), MEM_GAME);
    LOG_INFO("Game Init");
    return game;
}

b8 game_update(game_t *game, f64 delta)
{
    (void)game;
    (void)delta;
    return true;
}

b8 game_render(game_t *game, f64 delta)
{
    (void)game;
    (void)delta;
    return true;
}

void game_kill(game_t *game)
{
    if (!game) return;
    FREE(game, sizeof(game_t), MEM_GAME);
    LOG_INFO("Game Kill");
}

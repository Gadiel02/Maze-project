#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"

/* Function prototypes */
void init_player(player_t *player);
void handle_input(game_t *game, const Uint8 *keystate, double delta_time);
void move_player(game_t *game, float dir_x, float dir_y, double delta_time);
void rotate_player(player_t *player, float angle);
bool check_goal_reached(game_t *game);

#endif /* PLAYER_H */

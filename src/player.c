#include "../inc/player.h"

void init_player(player_t *player) {
    /* Initial player position (should be in an empty cell) */
    player->pos_x = 1.5;
    player->pos_y = 1.5;
    
    /* Initial direction vector (1,0) - facing east */
    player->dir_x = 1.0;
    player->dir_y = 0.0;
    
    /* Camera plane perpendicular to direction */
    player->plane_x = 0.0;
    player->plane_y = 0.66; /* FOV is about 66 degrees */
}

void handle_input(game_t *game, const Uint8 *keystate, double delta_time) {
    player_t *player = &game->player;
    
    /* Handle movement with both WASD and arrow keys */
    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        /* Move forward */
        move_player(game, player->dir_x, player->dir_y, delta_time);
    }
    
    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        /* Move backward */
        move_player(game, -player->dir_x, -player->dir_y, delta_time);
    }
    
    if (keystate[SDL_SCANCODE_A]) {
        /* Strafe left - CORREGIDO: ahora mueve a la izquierda */
        move_player(game, player->dir_y, -player->dir_x, delta_time);
    }
    
    if (keystate[SDL_SCANCODE_D]) {
        /* Strafe right - CORREGIDO: ahora mueve a la derecha */
        move_player(game, -player->dir_y, player->dir_x, delta_time);
    }
    
    /* Handle rotation */
    if (keystate[SDL_SCANCODE_LEFT]) {
        /* Rotate left */
        rotate_player(player, -ROTATION_SPEED * delta_time);
    }
    
    if (keystate[SDL_SCANCODE_RIGHT]) {
        /* Rotate right */
        rotate_player(player, ROTATION_SPEED * delta_time);
    }
    
    /* Handle quit */
    if (keystate[SDL_SCANCODE_ESCAPE]) {
        game->state = GAME_QUIT;
    }
}

void move_player(game_t *game, float dir_x, float dir_y, double delta_time) {
    player_t *player = &game->player;
    float move_speed = MOVE_SPEED * delta_time;
    float new_x, new_y;
    int map_x, map_y;
    
    /* Calculate new position */
    new_x = player->pos_x + dir_x * move_speed;
    new_y = player->pos_y + dir_y * move_speed;
    
    /* Check for collision with walls and handle sliding */
    map_x = (int)new_x;
    map_y = (int)new_y;
    
    /* Check if the new position is inside a wall */
    if (map_x >= 0 && map_x < MAP_WIDTH && 
        map_y >= 0 && map_y < MAP_HEIGHT) {
        
        /* Try to move along X if possible */
        if (game->map[map_y][(int)player->pos_x] != CELL_WALL) {
            player->pos_y = new_y;
        }
        
        /* Try to move along Y if possible */
        if (game->map[(int)player->pos_y][map_x] != CELL_WALL) {
            player->pos_x = new_x;
        }
    }
}

void rotate_player(player_t *player, float angle) {
    float old_dir_x;
    float old_plane_x;
    
    /* Rotate direction vector and camera plane */
    old_dir_x = player->dir_x;
    player->dir_x = player->dir_x * cos(angle) - player->dir_y * sin(angle);
    player->dir_y = old_dir_x * sin(angle) + player->dir_y * cos(angle);
    
    old_plane_x = player->plane_x;
    player->plane_x = player->plane_x * cos(angle) - player->plane_y * sin(angle);
    player->plane_y = old_plane_x * sin(angle) + player->plane_y * cos(angle);
}

bool check_goal_reached(game_t *game) {
    int player_map_x = (int)game->player.pos_x;
    int player_map_y = (int)game->player.pos_y;
    
    /* Check if player is on a goal cell */
    if (player_map_x >= 0 && player_map_x < MAP_WIDTH && 
        player_map_y >= 0 && player_map_y < MAP_HEIGHT) {
        return (game->map[player_map_y][player_map_x] == CELL_GOAL);
    }
    
    return false;
}

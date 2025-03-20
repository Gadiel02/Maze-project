#include "../inc/raycasting.h"

void render_frame(game_t *game) {
    SDL_Rect floor_rect;
    
    /* Clear screen (ceiling) with a dark blue color */
    SDL_SetRenderDrawColor(game->renderer, 50, 50, 150, 255);
    SDL_RenderClear(game->renderer);
    
    /* Draw floor with a dark color */
    floor_rect.x = 0;
    floor_rect.y = WINDOW_HEIGHT / 2;
    floor_rect.w = WINDOW_WIDTH;
    floor_rect.h = WINDOW_HEIGHT / 2;
    
    SDL_SetRenderDrawColor(game->renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(game->renderer, &floor_rect);
    
    /* Cast rays and draw walls */
    cast_rays(game);
}

void cast_rays(game_t *game) {
    player_t *player = &game->player;
    int x;
    double camera_x, ray_dir_x, ray_dir_y;
    int map_x, map_y;
    double side_dist_x, side_dist_y;
    double delta_dist_x, delta_dist_y, perp_wall_dist;
    int step_x, step_y;
    int hit, side;
    int line_height, draw_start, draw_end;
    int red_intensity, texture_value;
    
    /* Cast a ray for each column of the screen */
    for (x = 0; x < WINDOW_WIDTH; x++) {
        /* Calculate ray position and direction */
        camera_x = 2 * x / (double)WINDOW_WIDTH - 1; /* x-coordinate in camera space */
        ray_dir_x = player->dir_x + player->plane_x * camera_x;
        ray_dir_y = player->dir_y + player->plane_y * camera_x;
        
        /* Which box of the map we're in */
        map_x = (int)player->pos_x;
        map_y = (int)player->pos_y;
        
        /* Length of ray from one x or y-side to next x or y-side */
        delta_dist_x = fabs(1 / ray_dir_x);
        delta_dist_y = fabs(1 / ray_dir_y);
        
        /* Was a wall hit? */
        hit = 0;
        
        /* Calculate step and initial side_dist */
        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (player->pos_x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - player->pos_x) * delta_dist_x;
        }
        
        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (player->pos_y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - player->pos_y) * delta_dist_y;
        }
        
        /* Perform DDA (Digital Differential Analysis) */
        while (hit == 0) {
            /* Jump to next map square, either in x-direction, or in y-direction */
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            
            /* Check if ray has hit a wall or the goal */
            if (map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT) {
                if (game->map[map_y][map_x] == CELL_WALL) {
                    hit = 1;
                } else if (game->map[map_y][map_x] == CELL_GOAL) {
                    hit = 2; /* Goal hit */
                }
            } else {
                /* Hit the edge of the map */
                hit = 1;
                map_x = (int)fmax(0, fmin(map_x, MAP_WIDTH - 1));
                map_y = (int)fmax(0, fmin(map_y, MAP_HEIGHT - 1));
            }
        }
        
        /* Calculate distance projected on camera direction */
        if (side == 0) {
            perp_wall_dist = (map_x - player->pos_x + (1 - step_x) / 2) / ray_dir_x;
        } else {
            perp_wall_dist = (map_y - player->pos_y + (1 - step_y) / 2) / ray_dir_y;
        }
        
        /* Calculate height of line to draw on screen */
        line_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
        
        /* Calculate lowest and highest pixel to fill in current stripe */
        draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_start < 0) {
            draw_start = 0;
        }
        
        draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_end >= WINDOW_HEIGHT) {
            draw_end = WINDOW_HEIGHT - 1;
        }
        
        /* Set color based on wall or goal */
        if (hit == 2) {
            /* Goal (green) */
            SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
        } else {
            /* Wall - use red with texture effect */
            /* Generate a texture pattern based on position */
            texture_value = (map_x * 37 + map_y * 13 + (int)(perp_wall_dist * 100)) % 30;
            
            /* Base intensity for red walls */
            red_intensity = 180 - texture_value * 2;
            if (side == 1) {
                red_intensity = (red_intensity * 2) / 3; /* Darker for y-side walls */
            }
            
            SDL_SetRenderDrawColor(game->renderer, 
                                  red_intensity,         /* Red component */
                                  texture_value * 2,     /* Green component (low) */
                                  texture_value,         /* Blue component (very low) */
                                  255);
        }
        
        /* Draw the vertical stripe */
        SDL_RenderDrawLine(game->renderer, x, draw_start, x, draw_end);
    }
}

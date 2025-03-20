#include "../inc/map.h"

bool load_map(game_t *game, const char *filename) {
    FILE *file;
    int x, y;
    int cell;
    
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open map file %s\n", filename);
        return false;
    }
    
    /* Read map data from file */
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (fscanf(file, "%1d", &cell) != 1) {
                fprintf(stderr, "Error: Invalid map format or size\n");
                fclose(file);
                return false;
            }
            game->map[y][x] = cell;
        }
    }
    
    fclose(file);
    return true;
}

void render_minimap(game_t *game, int minimap_size) {
    int tile_size;
    SDL_Rect minimap_bg;
    int x, y;
    SDL_Rect tile;
    int player_x, player_y;
    int player_size;
    SDL_Rect player_rect;
    int dir_end_x, dir_end_y;
    
    /* Calculate tile size based on minimap size */
    tile_size = minimap_size / MAP_WIDTH;
    
    /* Draw background for minimap */
    minimap_bg.x = 10;
    minimap_bg.y = 10;
    minimap_bg.w = tile_size * MAP_WIDTH;
    minimap_bg.h = tile_size * MAP_HEIGHT;
    
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(game->renderer, &minimap_bg);
    
    /* Draw the map tiles */
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            tile.x = 10 + x * tile_size;
            tile.y = 10 + y * tile_size;
            tile.w = tile_size;
            tile.h = tile_size;
            
            switch (game->map[y][x]) {
                case CELL_EMPTY:
                    SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
                    break;
                case CELL_WALL:
                    SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
                    break;
                case CELL_GOAL:
                    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
                    break;
            }
            
            SDL_RenderFillRect(game->renderer, &tile);
        }
    }
    
    /* Draw the player position */
    player_x = 10 + (int)(game->player.pos_x * tile_size);
    player_y = 10 + (int)(game->player.pos_y * tile_size);
    player_size = tile_size / 2;
    
    player_rect.x = player_x - player_size / 2;
    player_rect.y = player_y - player_size / 2;
    player_rect.w = player_size;
    player_rect.h = player_size;
    
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &player_rect);
    
    /* Draw player direction line */
    dir_end_x = player_x + (int)(game->player.dir_x * tile_size);
    dir_end_y = player_y + (int)(game->player.dir_y * tile_size);
    
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(game->renderer, player_x, player_y, dir_end_x, dir_end_y);
}

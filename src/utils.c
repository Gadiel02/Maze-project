#include "../inc/utils.h"

bool init_sdl(game_t *game) {
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    /* Create window */
    game->window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (game->window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    
    /* Create renderer */
    game->renderer = SDL_CreateRenderer(
        game->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    
    if (game->renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return false;
    }
    
    /* Initialize time tracking */
    game->last_frame_time = SDL_GetTicks();
    
    return true;
}

void cleanup(game_t *game) {
    /* Clean up SDL resources */
    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
    }
    
    if (game->window) {
        SDL_DestroyWindow(game->window);
    }
    
    SDL_Quit();
}

double get_delta_time(game_t *game) {
    Uint32 current_time = SDL_GetTicks();
    double delta_time = (current_time - game->last_frame_time) / 1000.0; /* Convert to seconds */
    
    /* Ensure delta time is reasonable (not too big, not zero) */
    if (delta_time > 0.1) {
        delta_time = 0.1; /* Cap at 100ms to prevent big jumps */
    } else if (delta_time < 0.001) {
        delta_time = 0.016; /* If near zero, use ~60fps as fallback */
    }
    
    game->last_frame_time = current_time;
    return delta_time;
}

void limit_fps(Uint32 frame_time) {
    /* Target ~60 FPS */
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
    
    /* Delay to maintain constant frame rate */
    if ((int)frame_time < FRAME_DELAY) {
        SDL_Delay((Uint32)(FRAME_DELAY - frame_time));
    }
}

void init_rain(game_t *game) {
    int i;
    
    /* Initialize rain drops with random positions */
    for (i = 0; i < MAX_RAINDROPS; i++) {
        game->raindrops[i].x = rand() % WINDOW_WIDTH;
        game->raindrops[i].y = rand() % WINDOW_HEIGHT;
        game->raindrops[i].speed = 3 + rand() % RAINDROP_SPEED;
        game->raindrops[i].length = 5 + rand() % 15;
    }
}

void update_rain(game_t *game) {
    int i;
    
    /* Update position of each raindrop */
    for (i = 0; i < MAX_RAINDROPS; i++) {
        /* Move raindrop down and slightly to the right (wind effect) */
        game->raindrops[i].y += game->raindrops[i].speed;
        game->raindrops[i].x += 1;
        
        /* If raindrop is out of screen, reset its position */
        if (game->raindrops[i].y > WINDOW_HEIGHT || 
            game->raindrops[i].x > WINDOW_WIDTH) {
            game->raindrops[i].x = rand() % WINDOW_WIDTH;
            game->raindrops[i].y = 0;
            game->raindrops[i].speed = 3 + rand() % RAINDROP_SPEED;
            game->raindrops[i].length = 5 + rand() % 15;
        }
    }
}

void render_rain(game_t *game) {
    int i;
    int end_x, end_y;
    
    /* Set drawing color for rain (light blue) */
    SDL_SetRenderDrawColor(game->renderer, 180, 200, 255, 160);
    
    /* Draw each raindrop as a line */
    for (i = 0; i < MAX_RAINDROPS; i++) {
        end_x = game->raindrops[i].x + 1;
        end_y = game->raindrops[i].y + game->raindrops[i].length;
        
        SDL_RenderDrawLine(
            game->renderer,
            game->raindrops[i].x,
            game->raindrops[i].y,
            end_x,
            end_y
        );
    }
}

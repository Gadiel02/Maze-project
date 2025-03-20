#include "../inc/defs.h"
#include "../inc/raycasting.h"
#include "../inc/map.h"
#include "../inc/player.h"
#include "../inc/utils.h"

int main(void) {
    game_t game;
    const Uint8 *keystate;
    Uint32 frame_start;
    double delta_time;
    Uint32 frame_time;
    SDL_Event event;
    Uint8 prev_m_state = 0;
    Uint8 prev_r_state = 0;
    
    /* Initialize SDL and game */
    if (!init_sdl(&game)) {
        return 1;
    }
    
    /* Initialize player */
    init_player(&game.player);
    
    /* Initialize rain */
    init_rain(&game);
    
    /* By default, show minimap and no rain */
    game.show_minimap = true;
    game.show_rain = false;
    
    /* Load map */
    if (!load_map(&game, "maps/map1.txt")) {
        cleanup(&game);
        return 1;
    }
    
    /* Game loop */
    game.state = GAME_RUNNING;
    while (game.state == GAME_RUNNING) {
        frame_start = SDL_GetTicks();
        
        /* Process input */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.state = GAME_QUIT;
            }
        }
        
        /* Get keyboard state */
        SDL_PumpEvents();
        keystate = SDL_GetKeyboardState(NULL);
        
        /* Handle input and update player position */
        delta_time = get_delta_time(&game);
        handle_input(&game, keystate, delta_time);
        
        /* Toggle minimap with M key (only on key press, not hold) */
        if (keystate[SDL_SCANCODE_M] && !prev_m_state) {
            game.show_minimap = !game.show_minimap;
        }
        prev_m_state = keystate[SDL_SCANCODE_M];
        
        /* Toggle rain with R key (only on key press, not hold) */
        if (keystate[SDL_SCANCODE_R] && !prev_r_state) {
            game.show_rain = !game.show_rain;
        }
        prev_r_state = keystate[SDL_SCANCODE_R];
        
        /* Check if player reached the goal */
        if (check_goal_reached(&game)) {
            game.state = GAME_GOAL_REACHED;
        }
        
        /* Update rain if enabled */
        if (game.show_rain) {
            update_rain(&game);
        }
        
        /* Render the frame */
        render_frame(&game);
        
        /* Add a minimap in the corner if enabled */
        if (game.show_minimap) {
            render_minimap(&game, 150);
        }
        
        /* Render rain if enabled */
        if (game.show_rain) {
            render_rain(&game);
        }
        
        /* Present the rendered frame */
        SDL_RenderPresent(game.renderer);
        
        /* Cap framerate */
        frame_time = SDL_GetTicks() - frame_start;
        limit_fps(frame_time);
    }
    
    /* Display message if goal reached */
    if (game.state == GAME_GOAL_REACHED) {
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game.renderer);
        
        /* We would display a message here in a real game */
        /* For simplicity, just display a clear screen for 2 seconds */
        SDL_RenderPresent(game.renderer);
        SDL_Delay(2000);
    }
    
    /* Clean up and exit */
    cleanup(&game);
    return 0;
}

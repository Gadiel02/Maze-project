#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Window settings */
#define WINDOW_TITLE "Raycasting 3D Maze"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/* Map settings */
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define CELL_EMPTY 0
#define CELL_WALL 1
#define CELL_GOAL 2

/* Player settings */
#define MOVE_SPEED 1.5     /* Velocidad extremadamente alta */
#define ROTATION_SPEED 1.2 /* Rotación muy rápida */

/* Raycasting settings */
#define FOV 60.0
#define FOV_RAD (FOV * M_PI / 180.0)
#define NUM_RAYS WINDOW_WIDTH

/* Rain settings */
#define MAX_RAINDROPS 500
#define RAINDROP_SPEED 5

/* Game state */
typedef enum {
    GAME_RUNNING,
    GAME_GOAL_REACHED,
    GAME_QUIT
} game_state_t;

/* Player structure */
typedef struct {
    float pos_x, pos_y;     /* Player position */
    float dir_x, dir_y;     /* Direction vector */
    float plane_x, plane_y; /* Camera plane */
} player_t;

/* Raindrop structure */
typedef struct {
    int x, y;        /* Position */
    int speed;       /* Fall speed */
    int length;      /* Streak length */
} raindrop_t;

/* Game structure */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int map[MAP_HEIGHT][MAP_WIDTH];
    player_t player;
    game_state_t state;
    Uint32 last_frame_time;
    bool show_minimap;          /* Toggle for minimap visibility */
    bool show_rain;             /* Toggle for rain effect */
    raindrop_t raindrops[MAX_RAINDROPS]; /* Array of raindrops */
} game_t;

#endif /* DEFS_H */

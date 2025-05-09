#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "player.h"
#include "asteroids.h"
#include "bullet.h"

// Game states
typedef enum GameState {
    MENU,
    GAMEPLAY,
    GAME_OVER
} GameState;

// Game Architecture
typedef struct Game {
    GameState state;
    int score;
    Player player;           // still missing needs to be implemented in player.h first
    Asteroid asteroids[MAX_ASTEROIDS];
    Bullet bullets[MAX_BULLETS];
} Game;


/* 
 * If a bullet was just an array: -> {1, 2, 3, 4}
 * 
 * But if it a struct and an array of structs -> {[velocity, pos, radius, flightTime...], [velocity, pos, radius, flightTime], ..}
 */

// Function prototypes
void initGame(Game *game);
void UpdateGame(Game *game);
void DrawGame(Game *game);
void ResetGame(Game *game);

#endif                     // ending GAME_H config

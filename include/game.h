#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "player.h"
#include "asteroids.h"
#include "bullet.h"
#include "stars.h"                                        // included the stars.h wasnt present in v1.0

// Game states
typedef enum GameState {
    MAIN_MENU,
    GAMEPLAY,
    GAME_OVER,
    OPTIONS_MENU,                                         // NEW included, wasnt present in v1.0
    CONTROLS_MENU,                                        // NEW included, wasnt present in v1.0
    PAUSED                                                // NEW included, wasnt present in v1.0
} GameState;

// Adding game settings structure architecture, NEW, not present within the v1.0
typedef struct GameSettings
{
    bool soundEnabled;
    bool musicEnabled;
    bool showFPS;
    int difficulty;                                  // 0 - easy, 1 - normal, 2 - Hard
} GameSettings;

// Game Architecture
typedef struct Game {
    GameState state;
    int score;
    Player player;                                    // still missing needs to be implemented in player.h first
    Asteroid asteroids[MAX_ASTEROIDS];
    Bullet bullets[MAX_BULLETS];
    Star stars[MAX_STARS];                            // added the array of Star structures that we need

    // adding MENU related things
    int selectedOption;                               // used for tracking which menu option has been selected
    GameSettings settings;                            // structure containg game settings to the game

    int highScore;                                    // added additionally as well not present in v1.0
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

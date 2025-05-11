/*
* @Author: karlosiric
* @Date:   2025-05-09 09:11:58
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 15:49:06
*/

/*
 * Game.c is a c program responsible for generating and drawing the game and all of that so
 */

// Includes
#include <raylib.h>
#include <raymath.h>
#include "asteroids.h"
#include "bullet.h"
#include "menu.h"
#include "player.h"
#include "resolution.h"
#include "stars.h"
#include "utils.h"
#include "game.h"

const int RESOLUTIONS[4][2] = {
    {800, 600},          // 800x600
    {1024, 768},         // 1: 1024x768
    {1280, 720},         // 2: 1280x720 HD
    {1920, 1080}         // 3: 1920x1080 FULL HD
};

const char *RESOLUTION_NAMES[4] = {
    "800x600",
    "1024x768",
    "1280x720 (HD)",
    "1920x1080 (FHD)"
};




// We are passing the pointer to the game structure so we need to use -> in this case
void initGame(Game *game) 
{
    // dereferencing the pointer and using -> syntax in this case
    game->state = MAIN_MENU;                     // we initially set this to the MENU part of the game
    game->score = 0;                             // we then set the score to be equal to 0 for its own sake of the game
    game->highScore = 0;
    game->selectedOption = 0;                    // added new into this version, did not have it in v1.0
    game->settings.fullscreen = false;           // we initialize it to start with false at the start of the game

    // Initializing settings
    game->settings.soundEnabled = true;
    game->settings.musicEnabled = true;
    game->settings.showFPS = false;
    game->settings.difficulty = 1;


                                            // We initialize the player now
    InitPlayer(&game->player);

    InitAsteroid(game->asteroids);          // initialize the asteroids

    InitBullets(game->bullets);             // initialize the bullets

    InitStars(game->stars);                 // Initialize the stars, added new not present in v1.0

    InitResolutions(game);

    for (int i = 0; i < 5; i++)
    {
        SpawnAsteroids(game->asteroids);
    }
}

void UpdateGame(Game *game)
{
    // Check first if we should exit the application
    if (WindowShouldClose()) 
    {
        // This is the exit process triggered by the window X button
        // You may want to add confirmation here
        return;
    }

    // Handle pausing during gameplay - ONLY pause, don't exit
    if (game->state == GAMEPLAY && IsKeyPressed(KEY_P))
    {
        game->state = PAUSED;
        game->selectedOption = 0;   // Default to Resume
        return;
    }
    
    // Handle ESC during gameplay to return to main menu
    if (game->state == GAMEPLAY && IsKeyPressed(KEY_ESCAPE))
    {
        game->state = MAIN_MENU;
        game->selectedOption = 0;   // Default to first option
        return;
    }

    // Handle different game states
    switch (game->state)
    {
        case MAIN_MENU:
            UpdateMainMenu(game);
            UpdateStars(game->stars);
            break;

        case OPTIONS_MENU:
            UpdateOptionsMenu(game);
            UpdateStars(game->stars);
            break;

        case CONTROLS_MENU:
            UpdateControlsMenu(game);
            UpdateStars(game->stars);
            break;

        case PAUSED:
            UpdatePauseMenu(game);
            break;

        case GAMEPLAY:
            UpdatePlayer(&game->player, game->bullets); 
            UpdateAsteroid(game->asteroids);
            UpdateBullets(game->bullets);
            UpdateStars(game->stars);

            // We check the collisions
            checkCollisions(&game->player, game->asteroids, game->bullets, &game->score, &game->state);    
            break;

        case GAME_OVER:
            // Check for the high score
            if (game->score > game->highScore)
            {
                game->highScore = game->score;
            }

            // Now here we handle the restart or return to menu
            if (IsKeyPressed(KEY_ENTER))
            {
                ResetGame(game);
                game->state = GAMEPLAY;
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                ResetGame(game);
                game->state = MAIN_MENU;
                game->selectedOption = 0;
            }
            // Keep updating stars for visual effect
            UpdateStars(game->stars);
            break;
    }
}

// Now we need to actually draw the game
void DrawGame(Game *game) 
{
    // Always draw stars first for all states
    DrawStars(game->stars);

    switch (game->state) 
    {
        case MAIN_MENU:
            DrawMainMenu(game);
            break;

        case OPTIONS_MENU:
            DrawOptionsMenu(game);
            break;

        case CONTROLS_MENU:
            DrawControlsMenu(game);
            break;

        case GAMEPLAY:
            // Original gameplay drawing code
            DrawAsteroids(game->asteroids);
            DrawBullets(game->bullets);
            DrawPlayer(game->player);

            // For drawing the score on the screen
            DrawText(TextFormat("SCORE: %d", game->score), 10, 10, 20, WHITE);
            break;

        case PAUSED:
            // We need to make sure we Draw the game in the background
            DrawAsteroids(game->asteroids);
            DrawBullets(game->bullets);
            DrawPlayer(game->player);

            // Then draw the pause menu overlay
            DrawPauseMenu(game);
            break;

        case GAME_OVER:
            // Draw game over text
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, WHITE);
            DrawText(TextFormat("FINAL SCORE: %d", game->score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("FINAL SCORE: %d", game->score), 20) / 2, SCREEN_HEIGHT / 2, 20, WHITE);
            DrawText("Press ENTER to play again", SCREEN_WIDTH / 2 - MeasureText("Press ENTER to play again", 20) / 2, SCREEN_HEIGHT / 2 + 40, 20, WHITE);
            DrawText("Press ESC to return to menu", SCREEN_WIDTH / 2 - MeasureText("Press ESC to return to menu", 20) / 2, SCREEN_HEIGHT / 2 + 70, 20, WHITE);
            break;
    }

    // FIXED: Adding fps options enabled - properly use DrawFPS
    if (game->settings.showFPS) 
    {
        DrawFPS(10, SCREEN_HEIGHT - 30);
    }
}

// Implementing the reset game feature
void ResetGame(Game *game)
{
    // We reset the player
    InitPlayer(&game->player);

    InitAsteroid(game->asteroids);
    InitBullets(game->bullets);

    // now we spawn those initial asteroids once again
    for (int i = 0; i < 5; i++)
    {
        SpawnAsteroids(game->asteroids);
    }
    // reset the score finally
    game->score = 0; 
}

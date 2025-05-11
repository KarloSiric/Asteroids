/*
* @Author: karlosiric
* @Date:   2025-05-09 09:11:58
* @Last Modified by:   karlosiric
* @Last Modified time: 2025-05-11 18:16:46
*/

/*
 * Game.c is a c program responsible for generating and drawing the game and all of that so
 */

// Includes
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h> // Add this for NULL
#include "asteroids.h"
#include "bullet.h"
#include "menu.h"
#include "player.h"
#include "resolution.h"
#include "stars.h"
#include "utils.h"
#include "game.h"
#include "sound.h"

// External globals for screen dimensions
extern int screenWidth;
extern int screenHeight;

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

    InitResolutions(game);                  // Initialize resolutions AFTER other components

    // Initialize the sound manager (if it exists)
    if (game->soundManager != NULL) {
        ToggleSoundEnabled(game->soundManager, game->settings.soundEnabled);
        ToggleMusicEnabled(game->soundManager, game->settings.musicEnabled);
    }

    for (int i = 0; i < 5; i++)
    {
        SpawnAsteroids(game->asteroids);
    }
}

void UpdateGame(Game *game)
{
    // Update music if sound manager exists
    if (game->soundManager != NULL) {
        UpdateGameMusic(game->soundManager, game);
    }

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
        
        // Pause music when game is paused
        if (game->soundManager != NULL) {
            PauseGameMusic(game->soundManager);
        }
        
        return;
    }
    
    // Handle ESC during gameplay to return to main menu
    if (game->state == GAMEPLAY && IsKeyPressed(KEY_ESCAPE))
    {
        game->state = MAIN_MENU;
        game->selectedOption = 0;   // Default to first option
        
        // Play menu select sound
        if (game->soundManager != NULL && game->settings.soundEnabled) {
            PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
        }
        
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
            {  // Add braces to create a new scope for local variables
                // Store previous thrusting state to detect changes
                bool wasThrustingBefore = game->player.isThrusting;
                int previousShootCooldown = game->player.shootCooldown;
                
                UpdatePlayer(&game->player, game->bullets); 
                
                // Play thrust sound if player just started thrusting
                if (!wasThrustingBefore && game->player.isThrusting) {
                    if (game->soundManager != NULL && game->settings.soundEnabled) {
                        PlayGameSound(game->soundManager, SOUND_THRUST);
                    }
                }
                
                // Play shooting sound
                if (previousShootCooldown == 0 && game->player.shootCooldown > 0) {
                    if (game->soundManager != NULL && game->settings.soundEnabled) {
                        PlayGameSound(game->soundManager, SOUND_SHOOT);
                    }
                }
                
                UpdateAsteroid(game->asteroids);
                UpdateBullets(game->bullets);
                UpdateStars(game->stars);

                // We check the collisions - added sound support for collisions
                GameState previousState = game->state;
                int previousScore = game->score;
                
                checkCollisions(&game->player, game->asteroids, game->bullets, &game->score, &game->state);
                
                // If score changed, an asteroid was hit
                if (game->score > previousScore) {
                    if (game->soundManager != NULL && game->settings.soundEnabled) {
                        // Choose between small and large explosion sound randomly
                        if (GetRandomValue(0, 1) == 0) {
                            PlayGameSound(game->soundManager, SOUND_EXPLOSION_SMALL);
                        } else {
                            PlayGameSound(game->soundManager, SOUND_EXPLOSION_BIG);
                        }
                    }
                }
                
                // If state changed to GAME_OVER, player collided with asteroid
                if (previousState != GAME_OVER && game->state == GAME_OVER) {
                    if (game->soundManager != NULL && game->settings.soundEnabled) {
                        PlayGameSound(game->soundManager, SOUND_EXPLOSION_BIG);
                        PlayGameSound(game->soundManager, SOUND_GAME_OVER);
                    }
                }
            }
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
                
                // Play select sound
                if (game->soundManager != NULL && game->settings.soundEnabled) {
                    PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
                }
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                ResetGame(game);
                game->state = MAIN_MENU;
                game->selectedOption = 0;
                
                // Play select sound
                if (game->soundManager != NULL && game->settings.soundEnabled) {
                    PlayGameSound(game->soundManager, SOUND_MENU_SELECT);
                }
            }
            // Keep updating stars for visual effect
            UpdateStars(game->stars);
            break;
    }
}

// Helper function to draw centered text
static void DrawTextCenteredX(const char *text, int y, int fontSize, Color color) 
{
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, screenWidth/2 - textWidth/2, y, fontSize, color);
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
            DrawTextCenteredX("GAME OVER", screenHeight / 2 - 40, 40, WHITE);
            DrawTextCenteredX(TextFormat("FINAL SCORE: %d", game->score), screenHeight / 2, 20, WHITE);
            DrawTextCenteredX("Press ENTER to play again", screenHeight / 2 + 40, 20, WHITE);
            DrawTextCenteredX("Press ESC to return to menu", screenHeight / 2 + 70, 20, WHITE);
            break;
    }

    // FIXED: Adding fps options enabled - properly use DrawFPS
    if (game->settings.showFPS) 
    {
        DrawFPS(10, screenHeight - 30);
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
